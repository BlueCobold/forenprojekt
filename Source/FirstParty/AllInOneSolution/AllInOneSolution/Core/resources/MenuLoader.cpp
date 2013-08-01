#include "MenuLoader.hpp"
#include "ResourceManager.hpp"
#include "../gui/LineLabel.hpp"
 
MenuTemplate* MenuLoader::loadMenuTemplate(const std::string& path, ResourceManager& resourceManager)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path.c_str());

    if(doc.Error())
    {
        doc.PrintError();
        throw std::runtime_error(utility::replace(utility::translateKey("MenuFileInvalid"), path));
    }
    tinyxml2::XMLElement* menuXml = doc.FirstChildElement("menu");
    if(menuXml == nullptr)
        throw std::runtime_error(utility::replace(utility::translateKey("MenuEntryMissing"), path));

    MenuTemplate menu;
    MenuElements elements;
    if(auto background = menuXml->FirstChildElement("background"))
    {
        menu.background = sf::Sprite(*resourceManager.getTexture(background->Attribute("texture")));
        menu.background.setTextureRect(sf::IntRect(
            background->IntAttribute("srcx"), background->IntAttribute("srcy"),
            background->IntAttribute("width"), background->IntAttribute("height")));
    }
    if(auto caption = menuXml->FirstChildElement("caption"))
    {
        menu.captionFont = resourceManager.getBitmapFont(caption->Attribute("font"));
        menu.captionResourceKey = caption->Attribute("text");
        menu.captionOffset = sf::Vector2f(caption->FloatAttribute("offsetx"), caption->FloatAttribute("offsety"));
    }
    
    std::unordered_map<std::string, ButtonStyle> buttonStyles = parseButtonStyles(menuXml, resourceManager);
    std::unordered_map<std::string, CheckBoxStyle> checkboxStyles = parseCheckBoxStyles(menuXml, resourceManager);
    std::unordered_map<std::string, SliderStyle> sliderStyles = parseSliderStyles(menuXml, resourceManager);
    std::unordered_map<std::string, ToolTip> toolTip = parseToolTipStyle(menuXml, resourceManager);

    parseButtons(elements, menuXml, buttonStyles, toolTip, resourceManager);
    parseCheckBoxes(elements, menuXml, checkboxStyles, resourceManager);
    parseSliders(elements, menuXml, sliderStyles, resourceManager);
    parseLabels(elements, menuXml, resourceManager);
    parseImages(elements, menuXml, toolTip, resourceManager);
    parseSubWindow(menu, menuXml, resourceManager, toolTip, sliderStyles, checkboxStyles, buttonStyles);
    
    menu.menuElements = elements;

    return new MenuTemplate(menu);
}

void MenuLoader::parseButtons(
    MenuElements& elements, 
    tinyxml2::XMLElement* menuXml, 
    std::unordered_map<std::string, ButtonStyle>& buttonStyles, 
    std::unordered_map<std::string, ToolTip>& toolTip,
    ResourceManager& resourceManager)
{
    if(auto styles = menuXml->FirstChildElement("elements"))
    {
        for(auto buttonXml = styles->FirstChildElement("button");
            buttonXml != nullptr; buttonXml = buttonXml->NextSiblingElement("button"))
        {
            ButtonInfo button;
            auto style = buttonStyles.find(buttonXml->Attribute("style"));
            if(style == end(buttonStyles))
                throw std::runtime_error(utility::replace(utility::translateKey("UnknownButtonStyle"), buttonXml->Attribute("style")));
            button.style = style->second;
            button.position = sf::Vector2f(buttonXml->FloatAttribute("x"), buttonXml->FloatAttribute("y"));
            button.id = buttonXml->IntAttribute("id");
            auto text = buttonXml->Attribute("text");
            if(text != nullptr && std::string(text) != "")
                button.textResourceKey = utility::translateKey(text);
            else
                    button.textResourceKey = "";

            auto tooltipAvailable = buttonXml->Attribute("tooltip");
            if(tooltipAvailable != nullptr)
            {
                auto tooltip = toolTip.find(tooltipAvailable);
                if(tooltip == end(toolTip))
                    throw std::runtime_error(utility::replace(utility::translateKey("UnknownButtonToolTip"), buttonXml->Attribute("tooltip")));
                button.toolTip = tooltip->second;
                button.toolTip.setText(utility::translateKey(buttonXml->Attribute("tooltiptext")));
            }

            button.style.idleStyle.label = LineLabel(
                button.textResourceKey, 
                button.position, 0, button.style.idleStyle.font, LineLabel::Centered);
            button.style.idleStyle.label.setOffset(button.style.idleStyle.textOffset);

            button.style.hoverStyle.label = LineLabel(
                button.textResourceKey, 
                button.position, 0, button.style.hoverStyle.font, LineLabel::Centered);
            button.style.hoverStyle.label.setOffset(button.style.hoverStyle.textOffset);

            button.style.pressedStyle.label = LineLabel(
                button.textResourceKey, 
                button.position, 0, button.style.pressedStyle.font, LineLabel::Centered);
            button.style.pressedStyle.label.setOffset(button.style.pressedStyle.textOffset);
            elements.buttons.push_back(button);
        }
    }
}
void MenuLoader::parseCheckBoxes(
    MenuElements& elements, 
    tinyxml2::XMLElement* menuXml, 
    std::unordered_map<std::string, CheckBoxStyle>& checkBoxStyles, 
    ResourceManager& resourceManager)
{
    if(auto styles = menuXml->FirstChildElement("elements"))
    {
        for(auto checkboxXml = styles->FirstChildElement("checkbox");
            checkboxXml != nullptr; checkboxXml = checkboxXml->NextSiblingElement("checkbox"))
        {
            CheckBoxInfo checkBox;
            auto style = checkBoxStyles.find(checkboxXml->Attribute("style"));
            if(style == end(checkBoxStyles))
                throw std::runtime_error(utility::replace(utility::translateKey("UnknownCheckBoxStyle"), checkboxXml->Attribute("style")));
            checkBox.style = style->second;
            
            checkBox.position = sf::Vector2f(checkboxXml->FloatAttribute("x"), checkboxXml->FloatAttribute("y"));
            checkBox.id = checkboxXml->IntAttribute("id");

            elements.checkboxes.push_back(checkBox);
        }
    }
}
void MenuLoader::parseSliders(
    MenuElements& elements, 
    tinyxml2::XMLElement* menuXml, 
    std::unordered_map<std::string, SliderStyle>& sliderStyles,
    ResourceManager& resourceManager)
{
    if(auto styles = menuXml->FirstChildElement("elements"))
    {
        for(auto sliderXml = styles->FirstChildElement("slider");
            sliderXml != nullptr; sliderXml = sliderXml->NextSiblingElement("slider"))
        {
            SliderInfo slider;
            auto style = sliderStyles.find(sliderXml->Attribute("style"));
            if(style == end(sliderStyles))
                throw std::runtime_error(utility::replace(utility::translateKey("UnknownSliderStyle"), sliderXml->Attribute("style")));
            slider.style = style->second;
            slider.position = sf::Vector2f(sliderXml->FloatAttribute("x"), sliderXml->FloatAttribute("y"));
            slider.id = sliderXml->IntAttribute("id");

            elements.slider.push_back(slider);
        }
    }
}

void MenuLoader::parseLabels(
    MenuElements& elements,
    tinyxml2::XMLElement* menuXml,
    ResourceManager& resourceManager)
{
    if(auto styles = menuXml->FirstChildElement("elements"))
    {
        for(auto labelXml = styles->FirstChildElement("label");
            labelXml != nullptr; labelXml = labelXml->NextSiblingElement("label"))
        {
            LineLabel label(utility::translateKey(labelXml->Attribute("text")),
                            sf::Vector2f(0, 0),
                            0,
                            resourceManager.getBitmapFont(labelXml->Attribute("font")),
                            static_cast<LineLabel::Alignment>(labelXml->IntAttribute("aligment")),
                            labelXml->IntAttribute("id"));

            label.setOffset(sf::Vector2f(labelXml->FloatAttribute("x"), labelXml->FloatAttribute("y")));
            elements.labels.push_back(label);
        }
    }
}

void MenuLoader::parseImages(MenuElements& elements, 
                             tinyxml2::XMLElement* menuXml,
                             std::unordered_map<std::string, ToolTip>& toolTip,
                             ResourceManager& resourceManager)
{
    if(auto styles = menuXml->FirstChildElement("elements"))
    {
        for(auto imageXml = styles->FirstChildElement("image");
            imageXml != nullptr; imageXml = imageXml->NextSiblingElement("image"))
        {
            MenuSprite sprite(sf::Vector2f(0, 0),
                              sf::Vector2f(imageXml->FloatAttribute("x"), imageXml->FloatAttribute("y")),
                              imageXml->IntAttribute("id"));

            sprite.setTexture(*resourceManager.getTexture(imageXml->Attribute("texture")));
            sprite.setTextureRect(sf::IntRect(imageXml->IntAttribute("scrx"),
                                              imageXml->IntAttribute("scry"),
                                              imageXml->IntAttribute("width"),
                                              imageXml->IntAttribute("height")));
            
            if(auto toolTipName = imageXml->Attribute("tooltip"))
            {
                auto tooltip = toolTip.find(toolTipName);
                if(tooltip == end(toolTip))
                    throw std::runtime_error(utility::replace(utility::translateKey("UnknownToolTip"), imageXml->Attribute("tooltip")));
                tooltip->second.setText(utility::translateKey(imageXml->Attribute("tooltiptext")));
                sprite.setToolTip(tooltip->second);
            }
            elements.sprites.push_back(sprite);
        }
    }
}

void MenuLoader::parseSubWindow(MenuTemplate& menu,
                                tinyxml2::XMLElement* menuXml,
                                ResourceManager& resourceManager,
                                std::unordered_map<std::string, ToolTip>& toolTip,
                                std::unordered_map<std::string, SliderStyle>& sliderStyles,
                                std::unordered_map<std::string, CheckBoxStyle>& checkBoxStyles,
                                std::unordered_map<std::string, ButtonStyle>& buttonStyles)
{
    if(auto element = menuXml->FirstChildElement("elements"))
    {
        for(auto subXml = element->FirstChildElement("subwindow");
            subXml != nullptr; subXml = subXml->NextSiblingElement("subwindow"))
        {
            MenuElements ownElements;
            SubWindowInfo subWindowInfo;
            subWindowInfo.position = sf::Vector2f(subXml->FloatAttribute("x"), subXml->FloatAttribute("y"));
            subWindowInfo.size = sf::Vector2f(subXml->FloatAttribute("sizex"), subXml->FloatAttribute("sizey"));
            subWindowInfo.innerHeight = subXml->IntAttribute("innerheight");
            subWindowInfo.virtualPosition = sf::Vector2f(subXml->FloatAttribute("virtualx"), subXml->FloatAttribute("virtualy"));
            parseButtons(ownElements, subXml, buttonStyles, toolTip, resourceManager);
            parseCheckBoxes(ownElements, subXml, checkBoxStyles, resourceManager);
            parseSliders(ownElements, subXml, sliderStyles, resourceManager);
            parseLabels(ownElements, subXml, resourceManager);
            parseImages(ownElements, subXml, toolTip, resourceManager);
            subWindowInfo.menuElements = ownElements;
            menu.subWindow.push_back(subWindowInfo);
        }
    }
}
std::unordered_map<std::string, ButtonStyle> MenuLoader::parseButtonStyles(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager)
{
    std::unordered_map<std::string, ButtonStyle> buttonStyles;
    if(auto styles = menuXml->FirstChildElement("styles"))
    {
        std::unique_ptr<tinyxml2::XMLDocument> doc(new tinyxml2::XMLDocument);
        std::string filename = utility::toString("res/menus/") + styles->Attribute("source");
        doc->LoadFile(filename.c_str());     

        if(doc->Error())
        {
            doc->PrintError();
            throw std::runtime_error(utility::replace(utility::translateKey("IncludeFileInvalid"), filename));
        }

        for(auto styleXml = doc->FirstChildElement("styles")->FirstChildElement("buttonStyle");
            styleXml != nullptr; styleXml = styleXml->NextSiblingElement("buttonStyle"))
        {
            ButtonStyle style;
            style.idleStyle = loadButtonStateStyle(styleXml->FirstChildElement("idle"), resourceManager);
            style.hoverStyle = loadButtonStateStyle(styleXml->FirstChildElement("hover"), resourceManager);
            style.pressedStyle = loadButtonStateStyle(styleXml->FirstChildElement("pressed"), resourceManager);
            if(auto rect = styleXml->FirstChildElement("mouseRect"))
                style.mouseRect = sf::IntRect(
                    rect->IntAttribute("left"), rect->IntAttribute("top"),
                    rect->IntAttribute("width"), rect->IntAttribute("height"));
            buttonStyles[styleXml->Attribute("name")] = style;
        }
    }
    return buttonStyles;
}

ButtonStateStyle MenuLoader::loadButtonStateStyle(tinyxml2::XMLElement* xml, ResourceManager& resourceManager)
{
    ButtonStateStyle style;
    style.font = resourceManager.getBitmapFont(xml->Attribute("font"));
    style.textOffset = sf::Vector2f(xml->FloatAttribute("fontoffsetx"), xml->FloatAttribute("fontoffsety"));
    style.spriteOffset = sf::Vector2f(xml->FloatAttribute("offsetx"), xml->FloatAttribute("offsety"));
    style.sprite = sf::Sprite(*resourceManager.getTexture(xml->Attribute("texture")));
    style.sprite.setTextureRect(sf::IntRect(
            xml->IntAttribute("srcx"), xml->IntAttribute("srcy"),
            xml->IntAttribute("width"), xml->IntAttribute("height")));
    if(auto soundName = xml->Attribute("sound"))
        style.sound = std::shared_ptr<SoundObject>(new SoundObject(soundName, resourceManager.getSoundManager()));
    return style;
}

std::unordered_map<std::string, CheckBoxStyle> MenuLoader::parseCheckBoxStyles(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager)
{
    std::unordered_map<std::string, CheckBoxStyle> checkboxStyles;
    if(auto styles = menuXml->FirstChildElement("styles"))
    {
        std::unique_ptr<tinyxml2::XMLDocument> doc(new tinyxml2::XMLDocument);
        std::string filename = utility::toString("res/menus/") + styles->Attribute("source");
        doc->LoadFile(filename.c_str());     

        if(doc->Error())
        {
            doc->PrintError();
            throw std::runtime_error(utility::replace(utility::translateKey("IncludeFileInvalid"), filename));
        }

        for(auto styleXml = doc->FirstChildElement("styles")->FirstChildElement("checkboxStyle");
            styleXml != nullptr; styleXml = styleXml->NextSiblingElement("checkboxStyle"))
        {
            CheckBoxStyle style;
            
            style.uncheckedStyle = loadCheckBoxStateStyle(styleXml->FirstChildElement("unchecked"), resourceManager);
 
            style.checkedStyle = loadCheckBoxStateStyle(styleXml->FirstChildElement("checked"), resourceManager);
            if(auto rect = styleXml->FirstChildElement("mouseRect"))
                style.mouseRect = sf::IntRect(
                    rect->IntAttribute("left"), rect->IntAttribute("top"),
                    rect->IntAttribute("width"), rect->IntAttribute("height"));
            checkboxStyles[styleXml->Attribute("name")] = style;
        }
    }
    return checkboxStyles;
}

CheckBoxStateStyle MenuLoader::loadCheckBoxStateStyle(tinyxml2::XMLElement* xml, ResourceManager& resourceManager)
{
    CheckBoxStateStyle style;
    style.spriteOffset = sf::Vector2f(xml->FloatAttribute("offsetx"), xml->FloatAttribute("offsety"));
    style.sprite = sf::Sprite(*resourceManager.getTexture(xml->Attribute("texture")));
    style.sprite.setTextureRect(sf::IntRect(
            xml->IntAttribute("srcx"), xml->IntAttribute("srcy"),
            xml->IntAttribute("width"), xml->IntAttribute("height")));
    return style;
}

std::unordered_map<std::string, SliderStyle> MenuLoader::parseSliderStyles(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager)
{
    std::unordered_map<std::string, SliderStyle> sliderStyles;
    if(auto styles = menuXml->FirstChildElement("styles"))
    {
        std::unique_ptr<tinyxml2::XMLDocument> doc(new tinyxml2::XMLDocument);
        std::string filename = utility::toString("res/menus/") + styles->Attribute("source");
        doc->LoadFile(filename.c_str());     

        if(doc->Error())
        {
            doc->PrintError();
            throw std::runtime_error(utility::replace(utility::translateKey("IncludeFileInvalid"), filename));
        }

        for(auto styleXml = doc->FirstChildElement("styles")->FirstChildElement("sliderStyle");
            styleXml != nullptr; styleXml = styleXml->NextSiblingElement("sliderStyle"))
        {
            SliderStyle style;
            
            style.idle = loadSliderStateStyle(styleXml->FirstChildElement("idle"), resourceManager);
            style.active = loadSliderStateStyle(styleXml->FirstChildElement("active"), resourceManager);
            
            if(auto rect = styleXml->FirstChildElement("mouseRect"))
                style.mouseRect = sf::IntRect(
                    rect->IntAttribute("left"), rect->IntAttribute("top"),
                    rect->IntAttribute("width"), rect->IntAttribute("height"));
            if(auto value = styleXml->FirstChildElement("value"))
            {
                style.min = value->FloatAttribute("min");
                style.max = value->FloatAttribute("max");
                style.width = value->FloatAttribute("width");
            }
            sliderStyles[styleXml->Attribute("name")] = style;
        }
    }
    return sliderStyles;
}

SliderStateStyle MenuLoader::loadSliderStateStyle(tinyxml2::XMLElement* xml, ResourceManager& resourceManager)
{
    SliderStateStyle style;
    style.backgroundOffset = sf::Vector2f(xml->FloatAttribute("backroundoffsetx"), xml->FloatAttribute("backroundoffsety"));
    style.spriteBackround = sf::Sprite(*resourceManager.getTexture(xml->Attribute("backroundtexture")));
    style.spriteBackround.setTextureRect(sf::IntRect(
            xml->IntAttribute("backroundsrcx"), xml->IntAttribute("backroundsrcy"),
            xml->IntAttribute("backroundwidth"), xml->IntAttribute("backroundheight")));

    style.sliderOffset = sf::Vector2f(xml->FloatAttribute("slideroffsetx"), xml->FloatAttribute("slideroffsety"));
    style.spriteSlider = sf::Sprite(*resourceManager.getTexture(xml->Attribute("slidertexture")));
    style.spriteSlider.setTextureRect(sf::IntRect(
            xml->IntAttribute("slidersrcx"), xml->IntAttribute("slidersrcy"),
            xml->IntAttribute("sliderwidth"), xml->IntAttribute("sliderheight")));
    return style;
}

std::unordered_map<std::string, ToolTip> MenuLoader::parseToolTipStyle(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager)
{
    std::unordered_map<std::string, ToolTip> toolTip;
    if(auto styles = menuXml->FirstChildElement("styles"))
    {
        std::unique_ptr<tinyxml2::XMLDocument> doc(new tinyxml2::XMLDocument);
        std::string filename = utility::toString("res/menus/") + styles->Attribute("source");
        doc->LoadFile(filename.c_str());     

        if(doc->Error())
        {
            doc->PrintError();
            throw std::runtime_error(utility::replace(utility::translateKey("IncludeFileInvalid"), filename));
        }

        for(auto tooltipXml = doc->FirstChildElement("styles")->FirstChildElement("tooltipStyle");
            tooltipXml != nullptr; tooltipXml = tooltipXml->NextSiblingElement("tooltipStyle"))
        {
            sf::Sprite texture;
            std::unordered_map<int, sf::Sprite> backroundMap;
            int id = 0;
            int counter = 0;
            for(auto backround = tooltipXml->FirstChildElement("backround");
            backround != nullptr; backround = backround->NextSiblingElement("backround"))
            {
                id = backround->IntAttribute("id");
                texture.setTexture(*resourceManager.getTexture(backround->Attribute("texture")));
                texture.setTextureRect(sf::IntRect(backround->IntAttribute("srcx"),
                                                   backround->IntAttribute("srcy"),
                                                   backround->IntAttribute("width"),
                                                   backround->IntAttribute("height")));
                backroundMap[id] = texture;
                ++counter;
            }
            if(counter < 9)
                throw std::runtime_error(utility::translateKey("InvalidToolTipBackround")); 

            ToolTip tempToolTip("",
                                resourceManager.getBitmapFont(tooltipXml->FirstChildElement("text")->Attribute("font")),
                                sf::Vector2f(tooltipXml->FirstChildElement("text")->FloatAttribute("offsetx"),
                                             tooltipXml->FirstChildElement("text")->FloatAttribute("offsety")),
                                sf::Vector2f(tooltipXml->FloatAttribute("offsetx"), 
                                             tooltipXml->FloatAttribute("offsety")),
                                backroundMap);
            
            toolTip[tooltipXml->Attribute("name")] = tempToolTip;
        }
    }
    return toolTip;
}