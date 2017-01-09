
#include "AnimationParser.hpp"
#include "BlendingParser.hpp"
#include "MenuLoader.hpp"
#include "ResourceManager.hpp"
#include "SpriteSheet.hpp"
#include "../rendering/Sprite.hpp"
#include "../gui/AnimationContainer.hpp"
#include "../gui/Button.hpp"
#include "../gui/CheckBox.hpp"
#include "../gui/LineLabel.hpp"
#include "../gui/Slider.hpp"
#include "../gui/SubWindow.hpp"
#include "../MacHelper.hpp"

#include <array>
#include <vector>
#include <string>

Sprite getSprite(const std::string& prefix,
                 const tinyxml2::XMLElement* element,
                 ResourceManager& resourceManager)
{
    auto spriteSheetName = element->Attribute((prefix + "spriteSheet").c_str());
    auto spriteSheeet = spriteSheetName ? resourceManager.getSpriteSheet(spriteSheetName) : nullptr;
    if(auto spriteName = spriteSheeet ? element->Attribute((prefix + "sprite").c_str()) : nullptr)
    {
        auto sprite = spriteSheeet->get(spriteName);
        sprite.x += element->IntAttribute((prefix + "srcxoffset").c_str());
        sprite.y += element->IntAttribute((prefix + "srcyoffset").c_str());

        if(element->Attribute((prefix + "width").c_str()))
            sprite.width = element->IntAttribute((prefix + "width").c_str());
        if(element->Attribute((prefix + "widthoffset").c_str()))
            sprite.width += element->IntAttribute((prefix + "widthoffset").c_str());

        if(element->Attribute((prefix + "height").c_str()))
            sprite.height = element->IntAttribute((prefix + "height").c_str());
        if(element->Attribute((prefix + "heightoffset").c_str()))
            sprite.height += element->IntAttribute((prefix + "heightoffset").c_str());
        
        if(element->Attribute((prefix + "originx").c_str()))
            sprite.originX = element->FloatAttribute((prefix + "originx").c_str());
        if(element->Attribute((prefix + "originy").c_str()))
            sprite.originY = element->FloatAttribute((prefix + "originy").c_str());

        auto blending = sprite.blendMode;
        if(element->Attribute("blending"))
            blending = parseBlending(*element);

        Sprite baseSprite = Sprite(sf::Sprite(*resourceManager.getTexture(spriteSheeet->getTextureName()),
                                   sf::IntRect(sprite.x, sprite.y, sprite.width, sprite.height)),
                                   blending);

        bool useOrigin = false;
        element->QueryBoolAttribute("useOrigin", &useOrigin);
        if(useOrigin)
            baseSprite.setOrigin(sprite.originX, sprite.originY);

        return baseSprite;
    }
    else
    {
        return Sprite(sf::Sprite(*resourceManager.getTexture(element->Attribute((prefix + "texture").c_str())),
                                 sf::IntRect(element->IntAttribute((prefix + "srcx").c_str()),
                                     element->IntAttribute((prefix + "srcy").c_str()),
                                     element->IntAttribute((prefix + "width").c_str()),
                                     element->IntAttribute((prefix + "height").c_str()))),
                                     parseBlending(*element));
    }
}

Sprite getSprite(const tinyxml2::XMLElement* element,
                 ResourceManager& resourceManager)
{
    return getSprite("", element, resourceManager);
}

void parseComplexPositionPart(const char* attribute, float& relative, float& inverse, bool xAxis)
{
    if(!attribute)
        return;
    std::string val(attribute);
    auto pos = val.find(',');
    if(pos != std::string::npos)
    {
        std::vector<std::string> parts;
        parts.push_back(val.substr(0, pos));
        parts.push_back(val.substr(pos + 1));
        for(auto it = begin(parts); it != end(parts); ++it)
        {
            if(it->length() > 0 && (*it)[it->length() - 1] == (xAxis ? 'w' : 'h'))
                relative = utility::stringTo<float>(it->substr(0, it->length() - 1));
            else if(it->length() > 0 && (*it)[it->length() - 1] == (xAxis ? 'h' : 'w'))
                inverse = utility::stringTo<float>(it->substr(0, it->length() - 1));
            else
                relative = utility::stringTo<float>(*it);
        }
    }
    else
        relative = utility::stringTo<float>(val);
}

ScreenLocation parsePosition(const tinyxml2::XMLElement& element)
{
    sf::Vector2f relative, offset, inverse;
    parseComplexPositionPart(element.Attribute("x"), relative.x, inverse.x, true);
    parseComplexPositionPart(element.Attribute("y"), relative.y, inverse.y, false);
    element.QueryFloatAttribute("offsetx", &offset.x);
    element.QueryFloatAttribute("offsety", &offset.y);
    return ScreenLocation(relative, offset, sf::Vector2f(), inverse);
}

CloneHandler MenuLoader::_cloneHandler;

template<typename T>
void addAll(std::vector<std::unique_ptr<T>> source, std::vector<std::unique_ptr<MenuElement>>& target)
{
    for(auto it = begin(source); it != end(source); ++it)
        target.push_back(std::move(*it));
}

std::unique_ptr<MenuTemplate> MenuLoader::loadMenuTemplate(const std::string& path, ResourceManager& resourceManager)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile((resourcePath() + path).c_str());

    if(doc.Error())
    {
        doc.PrintError();
        throw std::runtime_error(utility::replace(utility::translateKey("@MenuFileInvalid"), path));
    }
    auto menuXml = doc.FirstChildElement("menu");
    if(menuXml == nullptr)
        throw std::runtime_error(utility::replace(utility::translateKey("@MenuEntryMissing"), path));

    auto menu = std::unique_ptr<MenuTemplate>(new MenuTemplate);

    std::unordered_map<std::string, ButtonStyle> buttonStyles = parseButtonStyles(menuXml, resourceManager);
    std::unordered_map<std::string, CheckBoxStyle> checkboxStyles = parseCheckBoxStyles(menuXml, resourceManager);
    std::unordered_map<std::string, SliderStyle> sliderStyles = parseSliderStyles(menuXml, resourceManager);
    std::unordered_map<std::string, ToolTip> toolTip = parseToolTipStyle(menuXml, resourceManager);
    std::unordered_map<std::string, InputBoxStyle> inputBoxStyle = parseInputBoxStyle(menuXml, resourceManager);

    addAll(parseButtons(menuXml, buttonStyles, toolTip, resourceManager), menu->menuElements);
    addAll(parseBorders(menuXml, resourceManager),  menu->menuElements);
    addAll(parseCheckBoxes(menuXml, checkboxStyles, toolTip, resourceManager), menu->menuElements);
    addAll(parseSliders(menuXml, sliderStyles, resourceManager), menu->menuElements);
    addAll(parseLabels(menuXml, resourceManager), menu->menuElements);
    addAll(parseInteractiveLabels(menuXml, toolTip, resourceManager), menu->menuElements);
    addAll(parseImages(menuXml, toolTip, resourceManager), menu->menuElements);
    addAll(parseInputBox(menuXml, inputBoxStyle, resourceManager), menu->menuElements);
    addAll(parseAnimationContainer(menuXml, resourceManager), menu->menuElements);
    addAll(parseSubWindow(menuXml, resourceManager, toolTip, sliderStyles, checkboxStyles, buttonStyles), menu->menuElements);
    
    return menu;
}

std::vector<std::unique_ptr<Button>> MenuLoader::parseButtons(
    const tinyxml2::XMLElement* menuXml, 
    const std::unordered_map<std::string, ButtonStyle>& buttonStyles, 
    const std::unordered_map<std::string, ToolTip>& toolTip,
    ResourceManager& resourceManager)
{
    std::vector<std::unique_ptr<Button>> elements;
    if(auto styles = menuXml->FirstChildElement("elements"))
    {
        for(auto buttonXml = styles->FirstChildElement("button");
            buttonXml != nullptr; buttonXml = buttonXml->NextSiblingElement("button"))
        {
            auto styleIt = buttonStyles.find(buttonXml->Attribute("style"));
            if(styleIt == end(buttonStyles))
                throw std::runtime_error(utility::replace(utility::translateKey("@UnknownButtonStyle"), buttonXml->Attribute("style")));
            auto style = styleIt->second;
            auto position = parsePosition(*buttonXml);
            auto id = buttonXml->IntAttribute("id");
            auto triggers = false;
            if(buttonXml->Attribute("triggering"))
                triggers = buttonXml->BoolAttribute("triggering");
            else
                triggers = true;

            auto text = buttonXml->Attribute("text");
            std::string textResourceKey;
            if(text != nullptr && std::string(text) != "")
                textResourceKey = text;

            style.idleStyle.label = LineLabel(
                textResourceKey, 
                ScreenLocation(position).addOffset(style.idleStyle.textOffset),
                0, style.idleStyle.font, LineLabel::Centered);

            style.hoverStyle.label = LineLabel(
                textResourceKey, 
                ScreenLocation(position).addOffset(style.hoverStyle.textOffset),
                0, style.hoverStyle.font, LineLabel::Centered);

            style.pressedStyle.label = LineLabel(
                textResourceKey, 
                ScreenLocation(position).addOffset(style.pressedStyle.textOffset),
                0, style.pressedStyle.font, LineLabel::Centered);

            auto button = std::unique_ptr<Button>(new Button(id, std::move(style), position, triggers));
            
            if(auto visibleWhenId = buttonXml->IntAttribute("visibleWhen"))
                button->setVisibleWhenId(visibleWhenId);

            auto tooltipAvailable = buttonXml->Attribute("tooltip");
            if(tooltipAvailable != nullptr)
            {
                auto tooltip = toolTip.find(tooltipAvailable);
                if(tooltip == end(toolTip))
                    throw std::runtime_error(utility::replace(utility::translateKey("@UnknownButtonToolTip"), buttonXml->Attribute("tooltip")));
                button->setToolTip(tooltip->second);
                button->setToolTipText(buttonXml->Attribute("tooltiptext"));
            }

            elements.push_back(std::move(button));
        }
    }
    return elements;
}

std::vector<std::unique_ptr<Border>> MenuLoader::parseBorders(
    const tinyxml2::XMLElement* menuXml,
    ResourceManager& resourceManager)
{
    std::vector<std::unique_ptr<Border>> elements;
    if(auto styles = menuXml->FirstChildElement("elements"))
    {
        for(auto borderXml = styles->FirstChildElement("border");
            borderXml != nullptr; borderXml = borderXml->NextSiblingElement("border"))
        {
            sf::Vector2f relativeSize(borderXml->FloatAttribute("widthPercent"), borderXml->FloatAttribute("heightPercent"));
            sf::Vector2f sizeOffset(borderXml->FloatAttribute("width"), borderXml->FloatAttribute("height"));
            auto id = borderXml->IntAttribute("id");

            std::unordered_map<Border::BackgroundId, Sprite> backgrounds;
            for(auto background = borderXml->FirstChildElement("background");
                background != nullptr; background = background->NextSiblingElement("background"))
            {
                auto id = static_cast<Border::BackgroundId>(background->IntAttribute("id"));
                auto sprite = getSprite(background, resourceManager);
                backgrounds[id] = sprite;
            }
            if(backgrounds.size() < 8)
                throw std::runtime_error(utility::replace(utility::translateKey("@InvalidBackground"), "Border"));
            
            std::array<std::vector<std::pair<Sprite, sf::Vector2f>>, 4> decos;
            for(auto deco = borderXml->FirstChildElement("deco");
                deco != nullptr; deco = deco->NextSiblingElement("deco"))
            {
                auto id = static_cast<Border::DecoId>(deco->IntAttribute("id") - 1);
                auto sprite = getSprite(deco, resourceManager);
                sf::Vector2f offset;
                deco->QueryFloatAttribute("offsetx", &offset.x);
                deco->QueryFloatAttribute("offsety", &offset.y);
                decos[id].push_back(std::make_pair(sprite, offset));
            }

            sf::FloatRect innerOffsets(0, 0, 0, 0);
            if(auto innerOffsetsXml = borderXml->FirstChildElement("innerOffsets"))
            {
                innerOffsetsXml->QueryFloatAttribute("x", &innerOffsets.left);
                innerOffsetsXml->QueryFloatAttribute("y", &innerOffsets.top);
                innerOffsetsXml->QueryFloatAttribute("width", &innerOffsets.width);
                innerOffsetsXml->QueryFloatAttribute("height", &innerOffsets.height);
            }
            std::unique_ptr<Border> border(new Border(id,
                                                      parsePosition(*borderXml),
                                                      ScreenSize(sizeOffset, relativeSize),
                                                      std::move(backgrounds),
                                                      innerOffsets,
                                                      std::move(decos)));
            
            sf::Vector2f scale(1, 1);
            borderXml->QueryFloatAttribute("scalex", &scale.x);
            borderXml->QueryFloatAttribute("scaley", &scale.y);
            bool keepAspectRatio = false;
            borderXml->QueryBoolAttribute("keepAspectRatio", &keepAspectRatio);
            border->setScale(scale, keepAspectRatio);

            elements.push_back(std::move(border));
        }
    }
    return elements;
}

std::vector<std::unique_ptr<CheckBox>> MenuLoader::parseCheckBoxes(
    const tinyxml2::XMLElement* menuXml, 
    const std::unordered_map<std::string, CheckBoxStyle>& checkBoxStyles,
    const std::unordered_map<std::string, ToolTip>& toolTip,
    ResourceManager& resourceManager)
{
    std::vector<std::unique_ptr<CheckBox>> elements;
    if(auto styles = menuXml->FirstChildElement("elements"))
    {
        for(auto checkboxXml = styles->FirstChildElement("checkbox");
            checkboxXml != nullptr; checkboxXml = checkboxXml->NextSiblingElement("checkbox"))
        {
            auto styleIt = checkBoxStyles.find(checkboxXml->Attribute("style"));
            if(styleIt == end(checkBoxStyles))
                throw std::runtime_error(utility::replace(utility::translateKey("@UnknownCheckBoxStyle"), checkboxXml->Attribute("style")));
            auto style = styleIt->second;
            
            auto id = checkboxXml->IntAttribute("id");

            auto checkBox = std::unique_ptr<CheckBox>(new CheckBox(id, style, parsePosition(*checkboxXml)));
            if(auto toolTipName = checkboxXml->Attribute("tooltip"))
            {
                auto tooltip = toolTip.find(toolTipName);
                if(tooltip == end(toolTip))
                    throw std::runtime_error(utility::replace(utility::translateKey("@UnknownToolTip"), checkboxXml->Attribute("tooltip")));
                checkBox->setToolTip(tooltip->second);
                checkBox->setToolTipText(checkboxXml->Attribute("tooltiptext"));
            }
            if(auto visibleWhenId = checkboxXml->IntAttribute("visibleWhen"))
                checkBox->setVisibleWhenId(visibleWhenId);

            elements.push_back(std::move(checkBox));
        }
    }
    return elements;
}

std::vector<std::unique_ptr<Slider>> MenuLoader::parseSliders(
    const tinyxml2::XMLElement* menuXml, 
    const std::unordered_map<std::string, SliderStyle>& sliderStyles, 
    ResourceManager& resourceManager)
{
    std::vector<std::unique_ptr<Slider>> elements;
    if(auto styles = menuXml->FirstChildElement("elements"))
    {
        for(auto sliderXml = styles->FirstChildElement("slider");
            sliderXml != nullptr; sliderXml = sliderXml->NextSiblingElement("slider"))
        {
            auto styleIt = sliderStyles.find(sliderXml->Attribute("style"));
            if(styleIt == end(sliderStyles))
                throw std::runtime_error(utility::replace(utility::translateKey("@UnknownSliderStyle"), sliderXml->Attribute("style")));
            auto style = styleIt->second;
            auto id = sliderXml->IntAttribute("id");
            
            auto slider = std::unique_ptr<Slider>(new Slider(id, style, parsePosition(*sliderXml)));
            if(auto visibleWhenId = sliderXml->IntAttribute("visibleWhen"))
                slider->setVisibleWhenId(visibleWhenId);
            
            elements.push_back(std::move(slider));
        }
    }
    return elements;
}

std::vector<std::unique_ptr<LineLabel>> MenuLoader::parseLabels(
        const tinyxml2::XMLElement* menuXml, 
        ResourceManager& resourceManager)
{
    std::vector<std::unique_ptr<LineLabel>> elements;
    if(auto styles = menuXml->FirstChildElement("elements"))
    {
        for(auto labelXml = styles->FirstChildElement("label");
            labelXml != nullptr; labelXml = labelXml->NextSiblingElement("label"))
        {
            auto label = std::unique_ptr<LineLabel>(new LineLabel(/*utility::translateKey(labelXml->Attribute("text")),*/
                            std::string(labelXml->Attribute("text")),
                            parsePosition(*labelXml),
                            0,
                            resourceManager.getBitmapFont(labelXml->Attribute("font")),
                            static_cast<LineLabel::Alignment>(labelXml->IntAttribute("alignment")),
                            labelXml->IntAttribute("id")));
            
            if(auto visibleWhenId = labelXml->IntAttribute("visibleWhen"))
                label->setVisibleWhenId(visibleWhenId);
            
            elements.push_back(std::move(label));
        }
    }
    return elements;
}

std::vector<std::unique_ptr<InteractiveLabel>> MenuLoader::parseInteractiveLabels(
        const tinyxml2::XMLElement* menuXml,
        const std::unordered_map<std::string, ToolTip>& toolTip,
        ResourceManager& resourceManager)
{
    std::vector<std::unique_ptr<InteractiveLabel>> elements;
    if(auto styles = menuXml->FirstChildElement("elements"))
    {
        for(auto labelXml = styles->FirstChildElement("ilabel");
            labelXml != nullptr; labelXml = labelXml->NextSiblingElement("ilabel"))
        {
            auto label = std::unique_ptr<InteractiveLabel>(new InteractiveLabel(
                            labelXml->Attribute("text"),
                            parsePosition(*labelXml),
                            0,
                            resourceManager.getBitmapFont(labelXml->Attribute("font")),
                            static_cast<LineLabel::Alignment>(labelXml->IntAttribute("alignment")),
                            labelXml->IntAttribute("id")));

            if(auto visibleWhenId = labelXml->IntAttribute("visibleWhen"))
                label->setVisibleWhenId(visibleWhenId);

            if(auto tooltipAvailable = labelXml->Attribute("tooltip"))
            {
                auto tooltip = toolTip.find(tooltipAvailable);
                if(tooltip == end(toolTip))
                    throw std::runtime_error(utility::replace(utility::translateKey("@UnknownButtonToolTip"), labelXml->Attribute("tooltip")));

                label->setToolTip(tooltip->second);
                label->setToolTipText(labelXml->Attribute("tooltiptext"));
            }

            elements.push_back(std::move(label));
        }
    }
    return elements;
}

std::vector<std::unique_ptr<MenuSprite>> MenuLoader::parseImages(
        const tinyxml2::XMLElement* menuXml,
        const std::unordered_map<std::string, ToolTip>& toolTip,
        ResourceManager& resourceManager)
{
    std::vector<std::unique_ptr<MenuSprite>> elements;
    if(auto styles = menuXml->FirstChildElement("elements"))
    {
        for(auto imageXml = styles->FirstChildElement("image");
            imageXml != nullptr; imageXml = imageXml->NextSiblingElement("image"))
        {
            auto id = imageXml->IntAttribute("id");
            Sprite baseSprite = getSprite(imageXml, resourceManager);
            sf::Vector2f relativeSize(imageXml->FloatAttribute("widthPercent"), imageXml->FloatAttribute("heightPercent"));
            sf::Vector2i texRect(baseSprite.getTextureRect().width, baseSprite.getTextureRect().height);
            ScreenSize size(sf::Vector2f(texRect), relativeSize);

            auto sprite = std::unique_ptr<MenuSprite>(new MenuSprite(
                                                          baseSprite,
                                                          parsePosition(*imageXml),
                                                          size,
                                                          id));
            
            sf::Vector2f scale(1, 1);
            imageXml->QueryFloatAttribute("scalex", &scale.x);
            imageXml->QueryFloatAttribute("scaley", &scale.y);
            bool keepAspectRatio = false;
            imageXml->QueryBoolAttribute("keepAspectRatio", &keepAspectRatio);
            sprite->setScale(scale, keepAspectRatio);

            auto toolTipText = imageXml->Attribute("tooltiptext");
            if(auto toolTipName = toolTipText ? imageXml->Attribute("tooltip") : nullptr)
            {
                auto tooltipIt = toolTip.find(toolTipName);
                if(tooltipIt == end(toolTip))
                    throw std::runtime_error(utility::replace(utility::translateKey("@UnknownToolTip"), imageXml->Attribute("tooltip")));
                auto tooltip = tooltipIt->second;
                tooltip.setText(toolTipText);
                sprite->setToolTip(tooltip);
            }

            if(auto visibleWhenId = imageXml->IntAttribute("visibleWhen"))
                sprite->setVisibleWhenId(visibleWhenId);

            elements.push_back(std::move(sprite));
        }
    }
    return elements;
}

std::vector<std::unique_ptr<SubWindow>> MenuLoader::parseSubWindow(
        const tinyxml2::XMLElement* menuXml,
        ResourceManager& resourceManager,
        const std::unordered_map<std::string, ToolTip>& toolTip,
        const std::unordered_map<std::string, SliderStyle>& sliderStyles,
        const std::unordered_map<std::string, CheckBoxStyle>& checkBoxStyles,
        const std::unordered_map<std::string, ButtonStyle>& buttonStyles)
{
    std::vector<std::unique_ptr<SubWindow>> elements;
    if(auto element = menuXml->FirstChildElement("elements"))
    {
        for(auto subXml = element->FirstChildElement("subwindow");
            subXml != nullptr; subXml = subXml->NextSiblingElement("subwindow"))
        {
            auto id = subXml->IntAttribute("id");
            SubWindowStyle style;
            if(auto styleXml = subXml->FirstChildElement("style"))
            {
                if(auto sprite = styleXml->FirstChildElement("scrollTop"))
                    style.scrollbarTop = getSprite(sprite, resourceManager).getSprite();

                if(auto sprite = styleXml->FirstChildElement("scrollMiddle"))
                    style.scrollbarMiddle = getSprite(sprite, resourceManager).getSprite();

                if(auto sprite = styleXml->FirstChildElement("scrollBottom"))
                    style.scrollbarBottom = getSprite(sprite, resourceManager).getSprite();

            }
            auto relativeSize = sf::Vector2f();
            subXml->QueryFloatAttribute("widthPercent", &relativeSize.x);
            subXml->QueryFloatAttribute("heightPercent", &relativeSize.y);
            auto size = sf::Vector2f(subXml->FloatAttribute("sizex"), subXml->FloatAttribute("sizey"));
            auto innerHeight = subXml->IntAttribute("innerheight");
            std::vector<std::unique_ptr<MenuElement>> subElements;
            addAll(parseButtons(subXml, buttonStyles, toolTip, resourceManager), subElements);
            addAll(parseBorders(subXml, resourceManager), subElements);
            addAll(parseCheckBoxes(subXml, checkBoxStyles, toolTip, resourceManager), subElements);
            addAll(parseSliders(subXml, sliderStyles, resourceManager), subElements);
            addAll(parseLabels(subXml, resourceManager), subElements);
            addAll(parseImages(subXml, toolTip, resourceManager), subElements);
            addAll(parseAnimationContainer(subXml, resourceManager), subElements);
            elements.push_back(std::unique_ptr<SubWindow>(new SubWindow(id, parsePosition(*subXml), ScreenSize(size, relativeSize), innerHeight, subElements, style)));
        }
    }
    return elements;
}

std::unordered_map<std::string, ButtonStyle> MenuLoader::parseButtonStyles(const tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager)
{
    std::unordered_map<std::string, ButtonStyle> buttonStyles;
    if(auto styles = menuXml->FirstChildElement("styles"))
    {
        tinyxml2::XMLDocument doc;
        std::string filename = resourcePath() + utility::toString("res/menus/") + styles->Attribute("source");
        doc.LoadFile(filename.c_str());

        if(doc.Error())
        {
            doc.PrintError();
            throw std::runtime_error(utility::replace(utility::translateKey("@IncludeFileInvalid"), filename));
        }

        for(auto styleXml = doc.FirstChildElement("styles")->FirstChildElement("buttonStyle");
            styleXml != nullptr; styleXml = styleXml->NextSiblingElement("buttonStyle"))
        {
            auto name = styleXml->Attribute("name");
            ButtonStyle style;
            style.idleStyle = std::move(loadButtonStateStyle(styleXml->FirstChildElement("idle"), resourceManager));
            style.hoverStyle = std::move(loadButtonStateStyle(styleXml->FirstChildElement("hover"), resourceManager));
            style.pressedStyle = std::move(loadButtonStateStyle(styleXml->FirstChildElement("pressed"), resourceManager));
            if(auto rect = styleXml->FirstChildElement("mouseRect"))
                style.mouseRect = sf::IntRect(
                    rect->IntAttribute("left"), rect->IntAttribute("top"),
                    rect->IntAttribute("width"), rect->IntAttribute("height"));
            buttonStyles.emplace(std::make_pair(name, std::move(style)));
        }
    }
    return buttonStyles;
}

ButtonStateStyle MenuLoader::loadButtonStateStyle(const tinyxml2::XMLElement* xml, ResourceManager& resourceManager)
{
    ButtonStateStyle style;
    style.font = resourceManager.getBitmapFont(xml->Attribute("font"));
    style.textOffset = sf::Vector2f(xml->FloatAttribute("fontoffsetx"), xml->FloatAttribute("fontoffsety"));
    style.spriteOffset = sf::Vector2f(xml->FloatAttribute("offsetx"), xml->FloatAttribute("offsety"));
    style.sprite = getSprite(xml, resourceManager);
    if(auto soundName = xml->Attribute("sound"))
        style.sound = std::shared_ptr<SoundObject>(new SoundObject(soundName, resourceManager.getSoundManager()));

    if(auto animationsXml = xml->FirstChildElement("animations"))
    {
        animationsXml->QueryBoolAttribute("resetOnExit", &style.resetOnExit);
        style.animation = std::unique_ptr<AnimationContainer>(new AnimationContainer(ScreenLocation(), 0, _cloneHandler));

        ProviderContext context(style.animation.get(), style.animation.get(), style.animation.get(), style.animation.get(), _cloneHandler);
        AnimationParser loader(AnimationContext(context, resourceManager, 0));
        auto animations = loader.parseMultiple(*animationsXml);
        for(auto it = begin(animations); it != end(animations); ++it)
            style.animation->bindAnimation(std::move(*it));
    }
    return style;
}

std::unordered_map<std::string, CheckBoxStyle> MenuLoader::parseCheckBoxStyles(const tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager)
{
    std::unordered_map<std::string, CheckBoxStyle> checkboxStyles;
    if(auto styles = menuXml->FirstChildElement("styles"))
    {
        tinyxml2::XMLDocument doc;
        std::string filename = resourcePath() + utility::toString("res/menus/") + styles->Attribute("source");
        doc.LoadFile(filename.c_str());

        if(doc.Error())
        {
            doc.PrintError();
            throw std::runtime_error(utility::replace(utility::translateKey("@IncludeFileInvalid"), filename));
        }

        for(auto styleXml = doc.FirstChildElement("styles")->FirstChildElement("checkboxStyle");
            styleXml != nullptr; styleXml = styleXml->NextSiblingElement("checkboxStyle"))
        {
            CheckBoxStyle style;
            
            style.uncheckedStyle = loadCheckBoxStateStyle(styleXml->FirstChildElement("unchecked"), resourceManager);
            style.checkedStyle = loadCheckBoxStateStyle(styleXml->FirstChildElement("checked"), resourceManager);
            style.hoverStyle = loadCheckBoxStateStyle(styleXml->FirstChildElement("hover"), resourceManager);
            if(auto checkedHover = styleXml->FirstChildElement("checkedHover"))
                style.checkedHoverStyle = loadCheckBoxStateStyle(checkedHover, resourceManager);
            else
                style.checkedHoverStyle = style.hoverStyle;

            if(auto rect = styleXml->FirstChildElement("mouseRect"))
                style.mouseRect = sf::IntRect(
                    rect->IntAttribute("left"), rect->IntAttribute("top"),
                    rect->IntAttribute("width"), rect->IntAttribute("height"));
            checkboxStyles[styleXml->Attribute("name")] = style;
        }
    }
    return checkboxStyles;
}

CheckBoxStateStyle MenuLoader::loadCheckBoxStateStyle(const tinyxml2::XMLElement* xml, ResourceManager& resourceManager)
{
    CheckBoxStateStyle style;
    style.spriteOffset = sf::Vector2f(xml->FloatAttribute("offsetx"), xml->FloatAttribute("offsety"));
    style.sprite = getSprite(xml, resourceManager);
    return style;
}

std::unordered_map<std::string, SliderStyle> MenuLoader::parseSliderStyles(const tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager)
{
    std::unordered_map<std::string, SliderStyle> sliderStyles;
    if(auto styles = menuXml->FirstChildElement("styles"))
    {
        tinyxml2::XMLDocument doc;
        std::string filename = resourcePath() + utility::toString("res/menus/") + styles->Attribute("source");
        doc.LoadFile(filename.c_str());

        if(doc.Error())
        {
            doc.PrintError();
            throw std::runtime_error(utility::replace(utility::translateKey("@IncludeFileInvalid"), filename));
        }

        for(auto styleXml = doc.FirstChildElement("styles")->FirstChildElement("sliderStyle");
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

SliderStateStyle MenuLoader::loadSliderStateStyle(const tinyxml2::XMLElement* xml, ResourceManager& resourceManager)
{
    SliderStateStyle style;
    style.backgroundOffset = sf::Vector2f(xml->FloatAttribute("backgroundoffsetx"), xml->FloatAttribute("backgroundoffsety"));
    style.background = getSprite("background", xml, resourceManager);

    style.buttonOffset = sf::Vector2f(xml->FloatAttribute("slideroffsetx"), xml->FloatAttribute("slideroffsety"));
    style.button = getSprite("slider", xml, resourceManager);
    return style;
}

std::unordered_map<std::string, ToolTip> MenuLoader::parseToolTipStyle(const tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager)
{
    std::unordered_map<std::string, ToolTip> toolTip;
    if(auto styles = menuXml->FirstChildElement("styles"))
    {
        tinyxml2::XMLDocument doc;
        std::string filename = resourcePath() + utility::toString("res/menus/") + styles->Attribute("source");
        doc.LoadFile(filename.c_str());

        if(doc.Error())
        {
            doc.PrintError();
            throw std::runtime_error(utility::replace(utility::translateKey("@IncludeFileInvalid"), filename));
        }

        for(auto tooltipXml = doc.FirstChildElement("styles")->FirstChildElement("tooltipStyle");
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
                texture = getSprite(background, resourceManager).getSprite();
                backgroundMap[id] = texture;
                ++counter;
            }
            if(counter < 9)
                throw std::runtime_error(utility::replace(utility::translateKey("@InvalidBackground"), "ToolTip"));

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

std::unordered_map<std::string, InputBoxStyle> MenuLoader::parseInputBoxStyle(const tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager)
{
    std::unordered_map<std::string, InputBoxStyle> inputBoxStyle;
    if(auto styles = menuXml->FirstChildElement("styles"))
    {
        tinyxml2::XMLDocument doc;
        std::string filename = resourcePath() + utility::toString("res/menus/") + styles->Attribute("source");
        doc.LoadFile(filename.c_str());

        if(doc.Error())
        {
            doc.PrintError();
            throw std::runtime_error(utility::replace(utility::translateKey("@IncludeFileInvalid"), filename));
        }

        for(auto inputBoxStyleXml = doc.FirstChildElement("styles")->FirstChildElement("inputboxStyle");
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
                throw std::runtime_error(utility::replace(utility::translateKey("@InvalidBackground"), "InputBox"));

            if(auto caret = inputBoxStyleXml->FirstChildElement("caret"))
            {
                inputBoxStyle[styleName].caret = getSprite(caret, resourceManager);
                inputBoxStyle[styleName].caretOffset = sf::Vector2f(caret->FloatAttribute("offsetX"), caret->FloatAttribute("offsetY"));
                inputBoxStyle[styleName].caretBlinkFrequency = caret->FloatAttribute("frequency");
            }
            else
                throw std::runtime_error(utility::translateKey("@NoCaret"));
        }
    }

    return inputBoxStyle;
}

std::vector<std::unique_ptr<InputBox>> MenuLoader::parseInputBox(
    const tinyxml2::XMLElement* menuXml,
    const std::unordered_map<std::string, InputBoxStyle>& inputBoxStyle,
    ResourceManager& resourceManager)
{
    std::vector<std::unique_ptr<InputBox>> elements;
    if(auto styles = menuXml->FirstChildElement("elements"))
    {
        for(auto inputBoxXml = styles->FirstChildElement("inputbox");
            inputBoxXml != nullptr; inputBoxXml = inputBoxXml->NextSiblingElement("inputbox"))
        {
            auto styleIt = inputBoxStyle.find(inputBoxXml->Attribute("style"));
            if(styleIt == end(inputBoxStyle))
                throw std::runtime_error(utility::replace(utility::translateKey("@UnknownInputBoxStyle"), inputBoxXml->Attribute("style")));
            
            auto style = styleIt->second;
            auto id = inputBoxXml->IntAttribute("id");
            auto inputLimit = inputBoxXml->IntAttribute("inputlimit");
            sf::Vector2f size(inputBoxXml->FloatAttribute("width"), inputBoxXml->FloatAttribute("height"));

            auto inputBox = std::unique_ptr<InputBox>(new InputBox(id, parsePosition(*inputBoxXml), size, inputLimit, style));

            if(auto visibleWhenId = inputBoxXml->IntAttribute("visibleWhen"))
                inputBox->setVisibleWhenId(visibleWhenId);

            elements.push_back(std::move(inputBox));
        }
    }
    return elements;
}

std::vector<std::unique_ptr<AnimationContainer>> MenuLoader::parseAnimationContainer(
        const tinyxml2::XMLElement* menuXml,
        ResourceManager& resourceManager)
{
    std::vector<std::unique_ptr<AnimationContainer>> elements;
    if(auto element = menuXml->FirstChildElement("elements"))
    {
        for(auto animationContainer = element->FirstChildElement("animationContainer");
            animationContainer != nullptr; animationContainer = animationContainer->NextSiblingElement("animationContainer"))
        {
            int id = animationContainer->IntAttribute("id");
            std::unique_ptr<AnimationContainer> animContainer(new AnimationContainer(parsePosition(*animationContainer), id, _cloneHandler));
            if(auto animationsXml = animationContainer->FirstChildElement("animations"))
            {
                ProviderContext context(animContainer.get(), animContainer.get(), animContainer.get(), animContainer.get(), _cloneHandler);
                AnimationParser loader(AnimationContext(context, resourceManager, 0));
                auto animations = loader.parseMultiple(*animationsXml);
                for(auto ani = begin(animations); ani != end(animations); ++ani)
                    animContainer->bindAnimation(std::move(*ani));
            }

            if(auto visibleWhenId = animationContainer->IntAttribute("visibleWhen"))
                animContainer->setVisibleWhenId(visibleWhenId);

            elements.push_back(std::move(animContainer));
        }
    }
    return elements;
}
