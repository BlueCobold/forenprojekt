
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
    std::unordered_map<std::string, ButtonSound> buttonSounds = parseSounds(menuXml, resourceManager);
    std::unordered_map<std::string, CheckBoxStyle> checkboxStyles = parseCheckBoxStyles(menuXml, resourceManager);
    std::unordered_map<std::string, SliderStyle> sliderStyles = parseSliderStyles(menuXml, resourceManager);

    parseButtons(menu, menuXml, buttonStyles, buttonSounds, resourceManager);
    parseCheckBoxes(menu, menuXml, checkboxStyles, resourceManager);
    parseSliders(menu, menuXml, sliderStyles, resourceManager);
    parseLabels(menu, menuXml, resourceManager);
    return new MenuTemplate(menu);
}

void MenuLoader::parseButtons(
    MenuTemplate& menu, 
    tinyxml2::XMLElement* menuXml, 
    std::unordered_map<std::string, ButtonStyle>& buttonStyles, 
    std::unordered_map<std::string, ButtonSound>& buttonSounds,
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
            auto sound = buttonSounds.find(buttonXml->Attribute("sound"));
            if(sound == end(buttonSounds))
                throw std::runtime_error(utility::replace(utility::translateKey("UnknownButtonSound"), buttonXml->Attribute("sound")));
            button.sound = sound->second;
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
            checkBox.textResourceKey = checkboxXml->Attribute("text");

            checkBox.style.uncheckedStyle.label = LineLabel(
                utility::translateKey(checkBox.textResourceKey), 
                checkBox.position, 0, checkBox.style.uncheckedStyle.font, LineLabel::Left);
            checkBox.style.uncheckedStyle.label.setOffset(checkBox.style.uncheckedStyle.textOffset);

            checkBox.style.checkedStyle.label = LineLabel(
                utility::translateKey(checkBox.textResourceKey), 
                checkBox.position, 0, checkBox.style.checkedStyle.font, LineLabel::Left);
            checkBox.style.checkedStyle.label.setOffset(checkBox.style.checkedStyle.textOffset);
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
            slider.textResourceKey = sliderXml->Attribute("text");

            slider.style.selected.label = LineLabel(
                utility::translateKey(slider.textResourceKey), 
                slider.position, 0, slider.style.selected.font, LineLabel::Left);
            slider.style.selected.label.setOffset(slider.style.selected.textOffset);

            slider.style.unselected.label = LineLabel(
                utility::translateKey(slider.textResourceKey), 
                slider.position, 0, slider.style.unselected.font, LineLabel::Left);
            slider.style.unselected.label.setOffset(slider.style.unselected.textOffset);

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
            LineLabel label;
            label.setBitmapFont(resourceManager.getBitmapFont(labelXml->Attribute("font")));
            label.setText(utility::translateKey(labelXml->Attribute("text")));
            label.setAlignment(static_cast<LineLabel::Alignment>(labelXml->IntAttribute("aligment")));
            label.setOffset(sf::Vector2f(labelXml->FloatAttribute("x"), labelXml->FloatAttribute("y")));
            menu.labels.push_back(label);
        }
    }
}
std::unordered_map<std::string, ButtonStyle> MenuLoader::parseButtonStyles(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager)
{
    std::unordered_map<std::string, ButtonStyle> buttonStyles;
    if(auto styles = menuXml->FirstChildElement("styles"))
    {
        for(auto styleXml = styles->FirstChildElement("buttonStyle");
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

std::unordered_map<std::string, ButtonSound> MenuLoader::parseSounds(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager)
{
    std::unordered_map<std::string, ButtonSound> buttonSounds;
    if(auto sounds = menuXml->FirstChildElement("sounds"))
    {
        for(auto soundXml = sounds->FirstChildElement("buttonSound");
            soundXml != nullptr; soundXml = soundXml->NextSiblingElement("buttonSound"))
        {
            ButtonSound sound;
            sound.hoverSound = sf::Sound(*resourceManager.getSoundBuffer(soundXml->FirstChildElement("hover")->Attribute("sound")));
            sound.hoverSound.setVolume(100.0f);
            buttonSounds[soundXml->Attribute("name")] = sound;
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
    if(auto styles = menuXml->FirstChildElement("styles"))
    {
        for(auto styleXml = styles->FirstChildElement("checkboxStyle");
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
    style.font = resourceManager.getBitmapFont(xml->Attribute("font"));
    style.textOffset = sf::Vector2f(xml->FloatAttribute("fontoffsetx"), xml->FloatAttribute("fontoffsety"));
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
        for(auto styleXml = styles->FirstChildElement("sliderStyle");
            styleXml != nullptr; styleXml = styleXml->NextSiblingElement("sliderStyle"))
        {
            SliderStyle style;
            
            style.unselected = loadSliderStateStyle(styleXml->FirstChildElement("unselected"), resourceManager);
            style.selected = loadSliderStateStyle(styleXml->FirstChildElement("selected"), resourceManager);
            
            if(auto rect = styleXml->FirstChildElement("mouseRect"))
                style.mouseRect = sf::IntRect(
                    rect->IntAttribute("left"), rect->IntAttribute("top"),
                    rect->IntAttribute("width"), rect->IntAttribute("height"));
            if(auto value = styleXml->FirstChildElement("value"))
            {
                style.min = value->FloatAttribute("min");
                style.max = value->FloatAttribute("max");
            }
            sliderStyles[styleXml->Attribute("name")] = style;
        }
    }
    return sliderStyles;
}

SliderStateStyle MenuLoader::loadSliderStateStyle(tinyxml2::XMLElement* xml, ResourceManager& resourceManager)
{
    SliderStateStyle style;
    style.font = resourceManager.getBitmapFont(xml->Attribute("font"));
    style.textOffset = sf::Vector2f(xml->FloatAttribute("fontoffsetx"), xml->FloatAttribute("fontoffsety"));
    style.spriteOffset = sf::Vector2f(xml->FloatAttribute("backroundoffsetx"), xml->FloatAttribute("backroundoffsety"));
    style.spriteBackround = sf::Sprite(*resourceManager.getTexture(xml->Attribute("backroundtexture")));
    style.spriteBackround.setTextureRect(sf::IntRect(
            xml->IntAttribute("backroundsrcx"), xml->IntAttribute("backroundsrcy"),
            xml->IntAttribute("backroundwidth"), xml->IntAttribute("backroundheight")));

    style.spriteSlider = sf::Sprite(*resourceManager.getTexture(xml->Attribute("slidertexture")));
    style.spriteSlider.setTextureRect(sf::IntRect(
            xml->IntAttribute("slidersrcx"), xml->IntAttribute("slidersrcy"),
            xml->IntAttribute("sliderwidth"), xml->IntAttribute("sliderheight")));
    return style;
}