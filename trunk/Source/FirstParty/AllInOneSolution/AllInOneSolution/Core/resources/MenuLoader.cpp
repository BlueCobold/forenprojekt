#include "MenuLoader.hpp"
#include "ResourceManager.hpp"
#include "../gui/LineLabel.hpp"
#include "LevelFileLoader.hpp"
#include "../MacHelper.hpp"

#include <string>

sf::Sprite getSprite(const std::string& prefix,
                     tinyxml2::XMLElement* element,
                     ResourceManager& resourceManager)
{
    auto spriteSheetName = element->Attribute((prefix+"spriteSheet").c_str());
    auto spriteSheeet = spriteSheetName ? resourceManager.getSpriteSheet(spriteSheetName) : nullptr;
    if(auto spriteName = spriteSheeet ? element->Attribute((prefix+"sprite").c_str()) : nullptr)
    {
        auto sprite = spriteSheeet->get(spriteName);
        sprite.x += element->IntAttribute((prefix+"srcxoffset").c_str());
        sprite.y += element->IntAttribute((prefix+"srcyoffset").c_str());

        if(element->Attribute((prefix+"width").c_str()))
            sprite.width = element->IntAttribute((prefix+"width").c_str());

        if(element->Attribute((prefix+"height").c_str()))
            sprite.height = element->IntAttribute((prefix+"height").c_str());

        sf::Sprite baseSprite = sf::Sprite(*resourceManager.getTexture(spriteSheeet->getTextureName()),
                                           sf::IntRect(sprite.x, sprite.y, sprite.width, sprite.height));
        return baseSprite;
    }
    else
    {
        return sf::Sprite(*resourceManager.getTexture(element->Attribute((prefix+"texture").c_str())),
                          sf::IntRect(element->IntAttribute((prefix+"srcx").c_str()),
                                      element->IntAttribute((prefix+"srcy").c_str()),
                                      element->IntAttribute((prefix+"width").c_str()),
                                      element->IntAttribute((prefix+"height").c_str())));
    }
}

sf::Sprite getSprite(tinyxml2::XMLElement* element,
                     ResourceManager& resourceManager)
{
    return getSprite("", element, resourceManager);
}

MenuTemplate* MenuLoader::loadMenuTemplate(const std::string& path, ResourceManager& resourceManager)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile((resourcePath() + path).c_str());

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
        menu.background = getSprite(background, resourceManager);

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
    std::unordered_map<std::string, InputBoxStyle> inputBoxStyle = parseInputBoxStyle(menuXml, resourceManager);

    parseButtons(elements, menuXml, buttonStyles, toolTip, resourceManager);
    parseCheckBoxes(elements, menuXml, checkboxStyles, toolTip, resourceManager);
    parseSliders(elements, menuXml, sliderStyles, resourceManager);
    parseLabels(elements, menuXml, resourceManager);
    parseImages(elements, menuXml, toolTip, resourceManager);
    parseInputBox(elements, menuXml, inputBoxStyle, resourceManager);
    parseAnimationContainer(elements, menuXml, resourceManager);
    parseSubWindow(menu, menuXml, resourceManager, toolTip, sliderStyles, checkboxStyles, buttonStyles);
    
    menu.menuElements = std::move(elements);

    return new MenuTemplate(std::move(menu));
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
                button.position, button.style.idleStyle.textOffset,
                0, button.style.idleStyle.font, LineLabel::Centered);

            button.style.hoverStyle.label = LineLabel(
                button.textResourceKey, 
                button.position, button.style.hoverStyle.textOffset,
                0, button.style.hoverStyle.font, LineLabel::Centered);

            button.style.pressedStyle.label = LineLabel(
                button.textResourceKey, 
                button.position, button.style.pressedStyle.textOffset,
                0, button.style.pressedStyle.font, LineLabel::Centered);
            elements.buttons.push_back(button);
        }
    }
}

void MenuLoader::parseCheckBoxes(
    MenuElements& elements, 
    tinyxml2::XMLElement* menuXml, 
    std::unordered_map<std::string, CheckBoxStyle>& checkBoxStyles,
    std::unordered_map<std::string, ToolTip>& toolTip,
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

            if(auto toolTipName = checkboxXml->Attribute("tooltip"))
            {
                auto tooltip = toolTip.find(toolTipName);
                if(tooltip == end(toolTip))
                    throw std::runtime_error(utility::replace(utility::translateKey("UnknownToolTip"), checkboxXml->Attribute("tooltip")));
                checkBox.toolTip = tooltip->second;
                checkBox.toolTip.setText(utility::translateKey(checkboxXml->Attribute("tooltiptext")));
            }

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
                            sf::Vector2f(labelXml->FloatAttribute("x"), labelXml->FloatAttribute("y")),
                            0,
                            resourceManager.getBitmapFont(labelXml->Attribute("font")),
                            static_cast<LineLabel::Alignment>(labelXml->IntAttribute("alignment")),
                            labelXml->IntAttribute("id"));

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
            sf::Sprite baseSprite = getSprite(imageXml, resourceManager);
            MenuSprite sprite(baseSprite,
                              sf::Vector2f(0, 0),
                              sf::Vector2f(imageXml->FloatAttribute("x"), imageXml->FloatAttribute("y")),
                              imageXml->IntAttribute("id"));

            auto toolTipText = imageXml->Attribute("tooltiptext");
            if(auto toolTipName = toolTipText ? imageXml->Attribute("tooltip") : nullptr)
            {
                auto tooltip = toolTip.find(toolTipName);
                if(tooltip == end(toolTip))
                    throw std::runtime_error(utility::replace(utility::translateKey("UnknownToolTip"), imageXml->Attribute("tooltip")));
                tooltip->second.setText(utility::translateKey(toolTipText));
                sprite.setToolTip(tooltip->second);
            }
            if(auto visibleWhenId = imageXml->IntAttribute("visibleWhen"))
                sprite.setVisibleWhenId(visibleWhenId);
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
            subWindowInfo.id = subXml->IntAttribute("id");
            if(auto style = subXml->FirstChildElement("style"))
            {
                if(auto sprite = style->FirstChildElement("scrollTop"))
                    subWindowInfo.style.scrollbarTop = getSprite(sprite, resourceManager);

                if(auto sprite = style->FirstChildElement("scrollMiddle"))
                    subWindowInfo.style.scrollbarMiddle = getSprite(sprite, resourceManager);

                if(auto sprite = style->FirstChildElement("scrollBottom"))
                    subWindowInfo.style.scrollbarBottom = getSprite(sprite, resourceManager);

            }
            subWindowInfo.position = sf::Vector2f(subXml->FloatAttribute("x"), subXml->FloatAttribute("y"));
            subWindowInfo.size = sf::Vector2f(subXml->FloatAttribute("sizex"), subXml->FloatAttribute("sizey"));
            subWindowInfo.innerHeight = subXml->IntAttribute("innerheight");
            parseButtons(ownElements, subXml, buttonStyles, toolTip, resourceManager);
            parseCheckBoxes(ownElements, subXml, checkBoxStyles, toolTip, resourceManager);
            parseSliders(ownElements, subXml, sliderStyles, resourceManager);
            parseLabels(ownElements, subXml, resourceManager);
            parseImages(ownElements, subXml, toolTip, resourceManager);
            parseAnimationContainer(ownElements, subXml, resourceManager);
            subWindowInfo.menuElements = std::move(ownElements);
            menu.subWindow.push_back(std::move(subWindowInfo));
        }
    }
}

std::unordered_map<std::string, ButtonStyle> MenuLoader::parseButtonStyles(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager)
{
    std::unordered_map<std::string, ButtonStyle> buttonStyles;
    if(auto styles = menuXml->FirstChildElement("styles"))
    {
        std::unique_ptr<tinyxml2::XMLDocument> doc(new tinyxml2::XMLDocument);
        std::string filename = resourcePath() + utility::toString("res/menus/") + styles->Attribute("source");
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
    style.sprite = getSprite(xml, resourceManager);
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
        std::string filename = resourcePath() + utility::toString("res/menus/") + styles->Attribute("source");
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
            style.hoverStyle = loadCheckBoxStateStyle(styleXml->FirstChildElement("hover"), resourceManager);

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
    style.sprite = getSprite(xml, resourceManager);
    return style;
}

std::unordered_map<std::string, SliderStyle> MenuLoader::parseSliderStyles(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager)
{
    std::unordered_map<std::string, SliderStyle> sliderStyles;
    if(auto styles = menuXml->FirstChildElement("styles"))
    {
        std::unique_ptr<tinyxml2::XMLDocument> doc(new tinyxml2::XMLDocument);
        std::string filename = resourcePath() + utility::toString("res/menus/") + styles->Attribute("source");
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
    style.backgroundOffset = sf::Vector2f(xml->FloatAttribute("backgroundoffsetx"), xml->FloatAttribute("backgroundoffsety"));
    style.spriteBackground = getSprite("background", xml, resourceManager);

    style.sliderOffset = sf::Vector2f(xml->FloatAttribute("slideroffsetx"), xml->FloatAttribute("slideroffsety"));
    style.spriteSlider = getSprite("slider", xml, resourceManager);
    return style;
}

std::unordered_map<std::string, ToolTip> MenuLoader::parseToolTipStyle(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager)
{
    std::unordered_map<std::string, ToolTip> toolTip;
    if(auto styles = menuXml->FirstChildElement("styles"))
    {
        std::unique_ptr<tinyxml2::XMLDocument> doc(new tinyxml2::XMLDocument);
        std::string filename = resourcePath() + utility::toString("res/menus/") + styles->Attribute("source");
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
            std::unordered_map<int, sf::Sprite> backgroundMap;
            int id = 0;
            int counter = 0;
            for(auto background = tooltipXml->FirstChildElement("background");
                background != nullptr; background = background->NextSiblingElement("background"))
            {
                id = background->IntAttribute("id");
                texture = getSprite(background, resourceManager);
                backgroundMap[id] = texture;
                ++counter;
            }
            if(counter < 9)
                throw std::runtime_error(utility::replace(utility::translateKey("InvalidBackground"), "ToolTip"));

            ToolTip tempToolTip("",
                                resourceManager.getBitmapFont(tooltipXml->FirstChildElement("text")->Attribute("font")),
                                sf::Vector2f(tooltipXml->FirstChildElement("text")->FloatAttribute("offsetx"),
                                             tooltipXml->FirstChildElement("text")->FloatAttribute("offsety")),
                                sf::Vector2f(tooltipXml->FloatAttribute("offsetx"), 
                                             tooltipXml->FloatAttribute("offsety")),
                                backgroundMap);
            
            toolTip[tooltipXml->Attribute("name")] = tempToolTip;
        }
    }
    return toolTip;
}

std::unordered_map<std::string, InputBoxStyle> MenuLoader::parseInputBoxStyle(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager)
{
    std::unordered_map<std::string, InputBoxStyle> inputBoxStyle;
    if(auto styles = menuXml->FirstChildElement("styles"))
    {
        std::unique_ptr<tinyxml2::XMLDocument> doc(new tinyxml2::XMLDocument);
        std::string filename = resourcePath() + utility::toString("res/menus/") + styles->Attribute("source");
        doc->LoadFile(filename.c_str());

        if(doc->Error())
        {
            doc->PrintError();
            throw std::runtime_error(utility::replace(utility::translateKey("IncludeFileInvalid"), filename));
        }

        for(auto inputBoxStyleXml = doc->FirstChildElement("styles")->FirstChildElement("inputboxStyle");
            inputBoxStyleXml != nullptr; inputBoxStyleXml = inputBoxStyleXml->NextSiblingElement("inputboxStyle"))
        {
            auto styleName = inputBoxStyleXml->Attribute("name");
            inputBoxStyle[styleName].font = resourceManager.getBitmapFont(inputBoxStyleXml->FirstChildElement("text")->Attribute("font"));
            inputBoxStyle[styleName].textOffset = sf::Vector2f(static_cast<float>(inputBoxStyleXml->FirstChildElement("text")->IntAttribute("offsetx")),
                                                               static_cast<float>(inputBoxStyleXml->FirstChildElement("text")->IntAttribute("offsety")));
            int id = 0;
            int counter = 0;
            for(auto background = inputBoxStyleXml->FirstChildElement("background");
                background != nullptr; background = background->NextSiblingElement("background"))
            {
                id = background->IntAttribute("id");
                inputBoxStyle[styleName].background[id] = getSprite(background, resourceManager);
                ++counter;
            }

            if(counter < 9)
                throw std::runtime_error(utility::replace(utility::translateKey("InvalidBackground"), "InputBox"));
        }
    }

    return inputBoxStyle;
}

void MenuLoader::parseInputBox(MenuElements& elements, tinyxml2::XMLElement* menuXml, std::unordered_map<std::string, InputBoxStyle>& inputBoxStyle, ResourceManager& resourceManager)
{
    if(auto styles = menuXml->FirstChildElement("elements"))
    {
        for(auto inputBoxXml = styles->FirstChildElement("inputbox");
            inputBoxXml != nullptr; inputBoxXml = inputBoxXml->NextSiblingElement("inputbox"))
        {
            InputBoxInfo inputBox;
            inputBox.style = inputBoxStyle[inputBoxXml->Attribute("style")];
            inputBox.id = inputBoxXml->IntAttribute("id");
            inputBox.inputLimit = inputBoxXml->IntAttribute("inputlimit");
            inputBox.position = sf::Vector2f(inputBoxXml->FloatAttribute("x"), inputBoxXml->FloatAttribute("y"));
            inputBox.size = sf::Vector2f(inputBoxXml->FloatAttribute("width"), inputBoxXml->FloatAttribute("height"));
            elements.infobox.push_back(inputBox);
        }
    }
}

void MenuLoader::parseAnimationContainer(
    MenuElements& elements,
    tinyxml2::XMLElement* menuXml,
    ResourceManager& resourceManager)
{
    if(auto element = menuXml->FirstChildElement("elements"))
    {
        if(auto animationContainer = element->FirstChildElement("animationContainer"))
        {
            int id = animationContainer->IntAttribute("id");
            sf::Vector2f position = sf::Vector2f(animationContainer->FloatAttribute("x"), animationContainer->FloatAttribute("y"));
            std::unique_ptr<AnimationContainer> animContainer(new AnimationContainer(position, id));
            std::unordered_map<std::string, tinyxml2::XMLElement*> functions;
            if(auto animations = animationContainer->FirstChildElement("animations"))
            {
                for(auto animation = animations->FirstChildElement("animation");
                    animation != nullptr; 
                    animation = animation->NextSiblingElement("animation"))
                    animContainer->bindAnimation(std::move(LevelFileLoader::parseAnimation(animation, animContainer.get(), animContainer.get(), resourceManager, &functions)));
            }
            elements.animationContainer.push_back(std::move(animContainer));
        }
    }
}