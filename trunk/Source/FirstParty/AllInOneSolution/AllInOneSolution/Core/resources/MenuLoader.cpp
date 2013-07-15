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
    std::unordered_map<std::string, SoundObject> buttonSounds = parseSounds(menuXml, resourceManager.getSoundManager());
    std::unordered_map<std::string, CheckBoxStyle> checkboxStyles = parseCheckBoxStyles(menuXml, resourceManager);
    std::unordered_map<std::string, SliderStyle> sliderStyles = parseSliderStyles(menuXml, resourceManager);
    std::unordered_map<std::string, ToolTip> toolTip = parseToolTip(menuXml, resourceManager);

    parseButtons(menu, menuXml, buttonStyles, buttonSounds, resourceManager);
    parseCheckBoxes(menu, menuXml, checkboxStyles, resourceManager);
    parseSliders(menu, menuXml, sliderStyles, resourceManager);
    parseLabels(menu, menuXml, resourceManager);
    parseImages(menu, menuXml, toolTip, resourceManager);
    return new MenuTemplate(menu);
}

void MenuLoader::parseButtons(
    MenuTemplate& menu, 
    tinyxml2::XMLElement* menuXml, 
    std::unordered_map<std::string, ButtonStyle>& buttonStyles, 
    std::unordered_map<std::string, SoundObject>& buttonSounds,
    ResourceManager& resourceManager)
{
    if(auto styles = menuXml->FirstChildElement("elements"))
    {
        for(auto buttonXml = styles->FirstChildElement("button");
            buttonXml != nullptr; buttonXml = buttonXml->NextSiblingElement("button"))
        {
            auto sound = buttonSounds.find(buttonXml->Attribute("sound"));
            if(sound == end(buttonSounds))
                throw std::runtime_error(utility::replace(utility::translateKey("UnknownButtonSound"), buttonXml->Attribute("sound")));
            ButtonInfo button(sound->second);

            auto style = buttonStyles.find(buttonXml->Attribute("style"));
            if(style == end(buttonStyles))
                throw std::runtime_error(utility::replace(utility::translateKey("UnknownButtonStyle"), buttonXml->Attribute("style")));
            button.style = style->second;
            button.position = sf::Vector2f(buttonXml->FloatAttribute("x"), buttonXml->FloatAttribute("y"));
            button.id = buttonXml->IntAttribute("id");
            button.textResourceKey = buttonXml->Attribute("text");

            button.style.idleStyle.label = LineLabel(
                utility::translateKey(button.textResourceKey), 
                button.position, 0, button.style.idleStyle.font, LineLabel::Centered);
            button.style.idleStyle.label.setOffset(button.style.idleStyle.textOffset);

            button.style.hoverStyle.label = LineLabel(
                utility::translateKey(button.textResourceKey), 
                button.position, 0, button.style.hoverStyle.font, LineLabel::Centered);
            button.style.hoverStyle.label.setOffset(button.style.hoverStyle.textOffset);

            button.style.pressedStyle.label = LineLabel(
                utility::translateKey(button.textResourceKey), 
                button.position, 0, button.style.pressedStyle.font, LineLabel::Centered);
            button.style.pressedStyle.label.setOffset(button.style.pressedStyle.textOffset);
            menu.buttons.push_back(button);
        }
    }
}
void MenuLoader::parseCheckBoxes(
    MenuTemplate& menu, 
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

            menu.checkboxes.push_back(checkBox);
        }
    }
}
void MenuLoader::parseSliders(
    MenuTemplate& menu, 
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

            menu.slider.push_back(slider);
        }
    }
}

void MenuLoader::parseLabels(MenuTemplate& menu, tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager)
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
            menu.labels.push_back(label);
        }
    }
}

void MenuLoader::parseImages(MenuTemplate& menu, 
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
            
            auto tooltip = toolTip.find(imageXml->Attribute("tooltip"));
            if(tooltip == end(toolTip))
                throw std::runtime_error(utility::replace(utility::translateKey("UnknownToolTip"), imageXml->Attribute("tooltip")));
            sprite.setToolTip(tooltip->second);

            menu.sprites.push_back(sprite);
        }
    }
}
std::unordered_map<std::string, ButtonStyle> MenuLoader::parseButtonStyles(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager)
{
    std::unordered_map<std::string, ButtonStyle> buttonStyles;
    if(auto styles = menuXml->FirstChildElement("styles")->FirstChildElement("buttonStyle"))
    {
        std::unique_ptr<tinyxml2::XMLDocument> doc(new tinyxml2::XMLDocument);
        std::string filename = utility::toString("res/menus/") + styles->Attribute("source");
        doc->LoadFile(filename.c_str());     

        if(doc->Error())
        {
            doc->PrintError();
            throw std::runtime_error(utility::replace(utility::translateKey("IncludeFileInvalid"), filename));
        }

        for(auto styleXml = doc->FirstChildElement("buttonStyle");
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

std::unordered_map<std::string, SoundObject> MenuLoader::parseSounds(tinyxml2::XMLElement* menuXml, SoundManager& soundManager)
{
    std::unordered_map<std::string, SoundObject> buttonSounds;
    if(auto sounds = menuXml->FirstChildElement("sounds"))
    {
        for(auto soundXml = sounds->FirstChildElement("buttonSound");
            soundXml != nullptr; soundXml = soundXml->NextSiblingElement("buttonSound"))
        {
            buttonSounds.insert(std::make_pair<std::string, SoundObject>(std::string(soundXml->Attribute("name")),
                SoundObject(soundXml->FirstChildElement("hover")->Attribute("sound"), soundManager)));
        }
    }
    return buttonSounds;
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
    return style;
}

std::unordered_map<std::string, CheckBoxStyle> MenuLoader::parseCheckBoxStyles(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager)
{
    std::unordered_map<std::string, CheckBoxStyle> checkboxStyles;
    if(auto styles = menuXml->FirstChildElement("styles")->FirstChildElement("checkboxStyle"))
    {
        std::unique_ptr<tinyxml2::XMLDocument> doc(new tinyxml2::XMLDocument);
        std::string filename = utility::toString("res/menus/") + styles->Attribute("source");
        doc->LoadFile(filename.c_str());     

        if(doc->Error())
        {
            doc->PrintError();
            throw std::runtime_error(utility::replace(utility::translateKey("IncludeFileInvalid"), filename));
        }

        for(auto styleXml = doc->FirstChildElement("checkboxStyle");
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
    if(auto styles = menuXml->FirstChildElement("styles")->FirstChildElement("sliderStyle"))
    {
        std::unique_ptr<tinyxml2::XMLDocument> doc(new tinyxml2::XMLDocument);
        std::string filename = utility::toString("res/menus/") + styles->Attribute("source");
        doc->LoadFile(filename.c_str());     

        if(doc->Error())
        {
            doc->PrintError();
            throw std::runtime_error(utility::replace(utility::translateKey("IncludeFileInvalid"), filename));
        }

        for(auto styleXml = doc->FirstChildElement("sliderStyle");
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

std::unordered_map<std::string, ToolTip> MenuLoader::parseToolTip(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager)
{
    std::unordered_map<std::string, ToolTip> toolTip;
    if(auto styles = menuXml->FirstChildElement("tooltips"))
    {
        for(auto tooltipXml = styles->FirstChildElement("tooltip");
            tooltipXml != nullptr; tooltipXml = tooltipXml->NextSiblingElement("tooltip"))
        {
            sf::Sprite leftTexture;
            sf::Sprite centerTexture;
            sf::Sprite rightTexture;
            leftTexture.setTexture(*resourceManager.getTexture(tooltipXml->FirstChildElement("left")->Attribute("texture")));
            leftTexture.setTextureRect(sf::IntRect(tooltipXml->FirstChildElement("left")->IntAttribute("srcx"),
                                                   tooltipXml->FirstChildElement("left")->IntAttribute("srcy"),
                                                   tooltipXml->FirstChildElement("left")->IntAttribute("width"),
                                                   tooltipXml->FirstChildElement("left")->IntAttribute("height")));
            centerTexture.setTexture(*resourceManager.getTexture(tooltipXml->FirstChildElement("center")->Attribute("texture")));
            centerTexture.setTextureRect(sf::IntRect(tooltipXml->FirstChildElement("center")->IntAttribute("srcx"),
                                                     tooltipXml->FirstChildElement("center")->IntAttribute("srcy"),
                                                     tooltipXml->FirstChildElement("center")->IntAttribute("width"),
                                                     tooltipXml->FirstChildElement("center")->IntAttribute("height")));
            rightTexture.setTexture(*resourceManager.getTexture(tooltipXml->FirstChildElement("right")->Attribute("texture")));
            rightTexture.setTextureRect(sf::IntRect(tooltipXml->FirstChildElement("right")->IntAttribute("srcx"),
                                                     tooltipXml->FirstChildElement("right")->IntAttribute("srcy"),
                                                     tooltipXml->FirstChildElement("right")->IntAttribute("width"),
                                                     tooltipXml->FirstChildElement("right")->IntAttribute("height")));
            
            ToolTip tempToolTip(utility::translateKey(tooltipXml->FirstChildElement("text")->Attribute("textkey")),
                                resourceManager.getBitmapFont(tooltipXml->FirstChildElement("text")->Attribute("font")),
                                sf::Vector2f(tooltipXml->FirstChildElement("text")->FloatAttribute("offsetx"),
                                             tooltipXml->FirstChildElement("text")->FloatAttribute("offsety")),
                                sf::Vector2f(tooltipXml->FloatAttribute("offsetx"), 
                                             tooltipXml->FloatAttribute("offsety")),
                                leftTexture,
                                centerTexture,
                                rightTexture);
            
            toolTip[tooltipXml->Attribute("name")] = tempToolTip;
        }
    }
    return toolTip;
}