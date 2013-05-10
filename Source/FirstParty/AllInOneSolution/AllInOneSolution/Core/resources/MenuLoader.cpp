
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
    std::unordered_map<std::string, ButtonStyle> buttonStyles = parseStyles(menuXml, resourceManager);
    parseButtons(menu, menuXml, buttonStyles, resourceManager);
    return new MenuTemplate(menu);
}

void MenuLoader::parseButtons(
    MenuTemplate& menu, 
    tinyxml2::XMLElement* menuXml, 
    std::unordered_map<std::string, ButtonStyle>& buttonStyles, 
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

std::unordered_map<std::string, ButtonStyle> MenuLoader::parseStyles(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager)
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
