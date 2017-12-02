
#include "AnimationParser.hpp"
#include "BlendingParser.hpp"
#include "MenuLoader.hpp"
#include "ResourceManager.hpp"
#include "SpriteSheet.hpp"
#include "../MacHelper.hpp"

#include <array>
#include <vector>
#include <string>

Sprite getSprite(const std::string& prefix,
                 const tinyxml2::XMLElement& element,
                 ResourceManager& resourceManager)
{
    auto spriteSheetName = element.Attribute((prefix + "spriteSheet").c_str());
    auto spriteSheeet = spriteSheetName ? resourceManager.getSpriteSheet(spriteSheetName) : nullptr;
    if(auto spriteName = spriteSheeet ? element.Attribute((prefix + "sprite").c_str()) : nullptr)
    {
        auto sprite = spriteSheeet->get(spriteName);
        sprite.x += element.IntAttribute((prefix + "srcxoffset").c_str());
        sprite.y += element.IntAttribute((prefix + "srcyoffset").c_str());

        if(element.Attribute((prefix + "width").c_str()))
            sprite.width = element.IntAttribute((prefix + "width").c_str());
        if(element.Attribute((prefix + "widthoffset").c_str()))
            sprite.width += element.IntAttribute((prefix + "widthoffset").c_str());

        if(element.Attribute((prefix + "height").c_str()))
            sprite.height = element.IntAttribute((prefix + "height").c_str());
        if(element.Attribute((prefix + "heightoffset").c_str()))
            sprite.height += element.IntAttribute((prefix + "heightoffset").c_str());
        
        if(element.Attribute((prefix + "originx").c_str()))
            sprite.originX = element.FloatAttribute((prefix + "originx").c_str());
        if(element.Attribute((prefix + "originy").c_str()))
            sprite.originY = element.FloatAttribute((prefix + "originy").c_str());

        auto blending = sprite.blendMode;
        if(element.Attribute("blending"))
            blending = parseBlending(element);

        Sprite baseSprite = Sprite(sf::Sprite(*resourceManager.getTexture(spriteSheeet->getTextureName()),
                                   sf::IntRect(sprite.x, sprite.y, sprite.width, sprite.height)),
                                   blending);

        bool useOrigin = false;
        element.QueryBoolAttribute("useOrigin", &useOrigin);
        if(useOrigin)
            baseSprite.setOrigin(sprite.originX, sprite.originY);

        return baseSprite;
    }
    else
    {
        return Sprite(sf::Sprite(*resourceManager.getTexture(element.Attribute((prefix + "texture").c_str())),
                                 sf::IntRect(element.IntAttribute((prefix + "srcx").c_str()),
                                     element.IntAttribute((prefix + "srcy").c_str()),
                                     element.IntAttribute((prefix + "width").c_str()),
                                     element.IntAttribute((prefix + "height").c_str()))),
                                     parseBlending(element));
    }
}

Sprite getSprite(const tinyxml2::XMLElement& element,
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

ScreenSize parseSize(const tinyxml2::XMLElement& element)
{
    sf::Vector2f relativeSize;
    element.QueryFloatAttribute("widthPercent", &relativeSize.x);
    element.QueryFloatAttribute("heightPercent", &relativeSize.y);
    sf::Vector2f sizeOffset;
    element.QueryFloatAttribute("widthOffset", &sizeOffset.x);
    element.QueryFloatAttribute("heightOffset", &sizeOffset.y);
    float aspectRatio = 0;
    element.QueryFloatAttribute("aspectRatio", &aspectRatio);
    ScreenSize::ScaleType scaleType = ScreenSize::XY;
    std::string sType;
    if(auto xmlType = element.Attribute("scaleType"))
        sType = std::string(xmlType);
    if(sType == "min")
        scaleType = ScreenSize::MIN;
    else if(sType == "max")
        scaleType = ScreenSize::MAX;
    return ScreenSize(sizeOffset, relativeSize, aspectRatio, scaleType);
}

ScreenScale parseScale(const tinyxml2::XMLElement& element)
{
    sf::Vector2f refSize(1, 1);
    element.QueryFloatAttribute("refWidth", &refSize.x);
    element.QueryFloatAttribute("refHeight", &refSize.y);
    sf::Vector2f percent;
    element.QueryFloatAttribute("growWidth", &percent.x);
    element.QueryFloatAttribute("growHeight", &percent.y);
    sf::Vector2f postScale(1, 1);
    element.QueryFloatAttribute("scalex", &postScale.x);
    element.QueryFloatAttribute("scaley", &postScale.y);
    std::string scaleType;
    if(auto type = element.Attribute("scaleType"))
        scaleType = std::string(type);
    auto type = ScreenScale::XY;
    if(scaleType == "min")
        type = ScreenScale::MIN;
    else if(scaleType == "max")
        type = ScreenScale::MAX;
    else if(scaleType == "xy")
        type = ScreenScale::XY;
    else if(scaleType == "xForBoth")
        type = ScreenScale::X_FOR_BOTH;
    else if(scaleType == "yForBoth")
        type = ScreenScale::Y_FOR_BOTH;
    return ScreenScale(percent, refSize, postScale, type);
}

CloneHandler MenuLoader::_cloneHandler;

template<typename T>
void addAll(std::vector<std::unique_ptr<T>> source, std::vector<std::unique_ptr<MenuElement>>& target)
{
    for(auto it = begin(source); it != end(source); ++it)
        target.push_back(std::move(*it));
}

template<typename T>
std::vector<std::unique_ptr<T>> parseElements(
    const char* name,
    const tinyxml2::XMLElement& menuXml,
    std::function<std::unique_ptr<T>(const tinyxml2::XMLElement& xml)> parser)
{
    std::vector<std::unique_ptr<T>> elements;
    if(auto element = menuXml.FirstChildElement("elements"))
    {
        for(auto subXml = element->FirstChildElement(name);
            subXml != nullptr; subXml = subXml->NextSiblingElement(name))
        {
            auto result = parser(*subXml);
            if(result)
                elements.push_back(std::move(result));
        }
    }
    return elements;
}

std::unique_ptr<MenuTemplate> MenuLoader::loadMenuTemplate(const std::string& path)
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

    parseButtonStyles(*menuXml);
    parseCheckBoxStyles(*menuXml);
    parseSliderStyles(*menuXml);
    parseToolTipStyles(*menuXml);
    parseInputBoxStyles(*menuXml);
    parseBorderStyles(*menuXml);

    addAll(parseButtons(*menuXml), menu->menuElements);
    addAll(parseBorders(*menuXml), menu->menuElements);
    addAll(parseCheckBoxes(*menuXml), menu->menuElements);
    addAll(parseCustomContents(*menuXml), menu->menuElements);
    addAll(parseSliders(*menuXml), menu->menuElements);
    addAll(parseLabels(*menuXml), menu->menuElements);
    addAll(parseInteractiveLabels(*menuXml), menu->menuElements);
    addAll(parseImages(*menuXml), menu->menuElements);
    addAll(parseInputBox(*menuXml), menu->menuElements);
    addAll(parseAnimationContainer(*menuXml), menu->menuElements);
    addAll(parseSubWindow(*menuXml), menu->menuElements);
    
    return menu;
}

std::vector<std::unique_ptr<Button>> MenuLoader::parseButtons(
    const tinyxml2::XMLElement& menuXml)
{
    return parseElements<Button>("button", menuXml,
        [&](const tinyxml2::XMLElement& buttonXml) -> std::unique_ptr<Button>
    {
        auto styleIt = m_buttonStyles.find(buttonXml.Attribute("style"));
        if(styleIt == std::end(m_buttonStyles))
            throw std::runtime_error(utility::replace(utility::translateKey("@UnknownButtonStyle"), buttonXml.Attribute("style")));
        auto style = styleIt->second;
        auto position = parsePosition(buttonXml);
        auto id = buttonXml.IntAttribute("id");
        auto triggers = false;
        if(buttonXml.Attribute("triggering"))
            triggers = buttonXml.BoolAttribute("triggering");
        else
            triggers = true;

        auto text = buttonXml.Attribute("text");
        std::string textResourceKey;
        if(text != nullptr && std::string(text) != "")
            textResourceKey = text;

        style.idleStyle.label = LineLabel(
            textResourceKey, 
            ScreenLocation(position).addOffset(style.idleStyle.textOffset),
            ScreenScale(), 
            0, style.idleStyle.font, LineLabel::Centered);

        style.hoverStyle.label = LineLabel(
            textResourceKey, 
            ScreenLocation(position).addOffset(style.hoverStyle.textOffset),
            ScreenScale(),
            0, style.hoverStyle.font, LineLabel::Centered);

        style.pressedStyle.label = LineLabel(
            textResourceKey, 
            ScreenLocation(position).addOffset(style.pressedStyle.textOffset),
            ScreenScale(),
            0, style.pressedStyle.font, LineLabel::Centered);

        auto button = std::unique_ptr<Button>(new Button(id, std::move(style), position, parseScale(buttonXml), triggers));

        if(buttonXml.Attribute("visibleWhen"))
            button->setVisibleWhenId(buttonXml.IntAttribute("visibleWhen"));
        if(buttonXml.Attribute("z"))
            button->setZLayer(buttonXml.IntAttribute("z"));

        auto tooltipAvailable = buttonXml.Attribute("tooltip");
        if(tooltipAvailable != nullptr)
        {
            auto tooltip = m_toolTips.find(tooltipAvailable);
            if(tooltip == std::end(m_toolTips))
                throw std::runtime_error(utility::replace(utility::translateKey("@UnknownButtonToolTip"), buttonXml.Attribute("tooltip")));
            button->setToolTip(tooltip->second);
            button->setToolTipText(buttonXml.Attribute("tooltiptext"));
        }

        sf::Vector2f scale(1, 1);
        buttonXml.QueryFloatAttribute("scalex", &scale.x);
        buttonXml.QueryFloatAttribute("scaley", &scale.y);

        return button;
    });
}

std::vector<std::unique_ptr<Border>> MenuLoader::parseBorders(
    const tinyxml2::XMLElement& menuXml)
{
    return parseElements<Border>("border", menuXml,
        [&](const tinyxml2::XMLElement& borderXml) -> std::unique_ptr<Border>
    {
        auto id = borderXml.IntAttribute("id");

        BorderStyle style;
        if(auto styleName = borderXml.Attribute("style"))
        {
            auto listedStyle = m_borderStyles.find(styleName);
            if(listedStyle != std::end(m_borderStyles))
                style = listedStyle->second;
        }
        parseStyle(borderXml, style);

        std::unique_ptr<Border> border(new Border(id,
                                                  parsePosition(borderXml),
                                                  parseSize(borderXml),
                                                  style));

        if(borderXml.Attribute("visibleWhen"))
            border->setVisibleWhenId(borderXml.IntAttribute("visibleWhen"));
        if(borderXml.Attribute("z"))
            border->setZLayer(borderXml.IntAttribute("z"));

        sf::Vector2f scale(1, 1);
        borderXml.QueryFloatAttribute("scalex", &scale.x);
        borderXml.QueryFloatAttribute("scaley", &scale.y);
        bool keepAspectRatio = false;
        borderXml.QueryBoolAttribute("keepAspectRatio", &keepAspectRatio);
        border->setScale(scale, keepAspectRatio);

        return border;
    });
}

std::vector<std::unique_ptr<CustomContent>> MenuLoader::parseCustomContents(
    const tinyxml2::XMLElement& menuXml)
{
    return parseElements<CustomContent>("customContent", menuXml,
        [&](const tinyxml2::XMLElement& xml) -> std::unique_ptr<CustomContent>
    {
        auto id = xml.IntAttribute("id");
        return std::unique_ptr<CustomContent>(new CustomContent(id));
    });
}

std::vector<std::unique_ptr<CheckBox>> MenuLoader::parseCheckBoxes(
    const tinyxml2::XMLElement& menuXml)
{
    return parseElements<CheckBox>("checkbox", menuXml,
        [&](const tinyxml2::XMLElement& checkboxXml) -> std::unique_ptr<CheckBox>
    {
        auto styleIt = m_checkBoxStyles.find(checkboxXml.Attribute("style"));
        if(styleIt == std::end(m_checkBoxStyles))
            throw std::runtime_error(utility::replace(utility::translateKey("@UnknownCheckBoxStyle"), checkboxXml.Attribute("style")));
        auto style = styleIt->second;
            
        auto id = checkboxXml.IntAttribute("id");

        auto checkBox = std::unique_ptr<CheckBox>(new CheckBox(id,
                                                               style,
                                                               parsePosition(checkboxXml),
                                                               parseScale(checkboxXml)));
        if(auto toolTipName = checkboxXml.Attribute("tooltip"))
        {
            auto tooltip = m_toolTips.find(toolTipName);
            if(tooltip == std::end(m_toolTips))
                throw std::runtime_error(utility::replace(utility::translateKey("@UnknownToolTip"), checkboxXml.Attribute("tooltip")));
            checkBox->setToolTip(tooltip->second);
            checkBox->setToolTipText(checkboxXml.Attribute("tooltiptext"));
        }

        if(checkboxXml.Attribute("visibleWhen"))
            checkBox->setVisibleWhenId(checkboxXml.IntAttribute("visibleWhen"));
        if(checkboxXml.Attribute("z"))
            checkBox->setZLayer(checkboxXml.IntAttribute("z"));

        return checkBox;
    });
}

std::vector<std::unique_ptr<Slider>> MenuLoader::parseSliders(
    const tinyxml2::XMLElement& menuXml)
{
    return parseElements<Slider>("slider", menuXml,
        [&](const tinyxml2::XMLElement& sliderXml) -> std::unique_ptr<Slider>
    {
        auto styleIt = m_sliderStyles.find(sliderXml.Attribute("style"));
        if(styleIt == std::end(m_sliderStyles))
            throw std::runtime_error(utility::replace(utility::translateKey("@UnknownSliderStyle"), sliderXml.Attribute("style")));
        auto style = styleIt->second;
        auto id = sliderXml.IntAttribute("id");

        auto slider = std::unique_ptr<Slider>(new Slider(id, style, parsePosition(sliderXml)));
        if(sliderXml.Attribute("visibleWhen"))
            slider->setVisibleWhenId(sliderXml.IntAttribute("visibleWhen"));
        if(sliderXml.Attribute("z"))
            slider->setZLayer(sliderXml.IntAttribute("z"));

        return slider;
    });
}

std::vector<std::unique_ptr<LineLabel>> MenuLoader::parseLabels(
    const tinyxml2::XMLElement& menuXml)
{
    return parseElements<LineLabel>("label", menuXml,
        [&](const tinyxml2::XMLElement& labelXml) -> std::unique_ptr<LineLabel>
    {
        auto label = std::unique_ptr<LineLabel>(new LineLabel(
                        std::string(labelXml.Attribute("text")),
                        parsePosition(labelXml),
                        parseScale(labelXml),
                        0,
                        m_resourceManager.getBitmapFont(labelXml.Attribute("font")),
                        static_cast<LineLabel::Alignment>(labelXml.IntAttribute("alignment")),
                        labelXml.IntAttribute("id")));

        if(labelXml.Attribute("visibleWhen"))
            label->setVisibleWhenId(labelXml.IntAttribute("visibleWhen"));
        if(labelXml.Attribute("z"))
            label->setZLayer(labelXml.IntAttribute("z"));

        return label;
    });
}

std::vector<std::unique_ptr<InteractiveLabel>> MenuLoader::parseInteractiveLabels(
    const tinyxml2::XMLElement& menuXml)
{
    return parseElements<InteractiveLabel>("ilabel", menuXml,
        [&](const tinyxml2::XMLElement& labelXml) -> std::unique_ptr<InteractiveLabel>
    {
        auto label = std::unique_ptr<InteractiveLabel>(new InteractiveLabel(
                        labelXml.Attribute("text"),
                        parsePosition(labelXml),
                        0,
                        m_resourceManager.getBitmapFont(labelXml.Attribute("font")),
                        static_cast<LineLabel::Alignment>(labelXml.IntAttribute("alignment")),
                        labelXml.IntAttribute("id")));

        if(labelXml.Attribute("visibleWhen"))
            label->setVisibleWhenId(labelXml.IntAttribute("visibleWhen"));
        if(labelXml.Attribute("z"))
            label->setZLayer(labelXml.IntAttribute("z"));

        if(auto tooltipAvailable = labelXml.Attribute("tooltip"))
        {
            auto tooltip = m_toolTips.find(tooltipAvailable);
            if(tooltip == std::end(m_toolTips))
                throw std::runtime_error(utility::replace(utility::translateKey("@UnknownButtonToolTip"), labelXml.Attribute("tooltip")));

            label->setToolTip(tooltip->second);
            label->setToolTipText(labelXml.Attribute("tooltiptext"));
        }

        return label;
    });
}

std::vector<std::unique_ptr<MenuSprite>> MenuLoader::parseImages(
    const tinyxml2::XMLElement& menuXml)
{
    return parseElements<MenuSprite>("image", menuXml,
        [&](const tinyxml2::XMLElement& imageXml) -> std::unique_ptr<MenuSprite>
    {
        auto id = imageXml.IntAttribute("id");
        Sprite baseSprite = getSprite(imageXml, m_resourceManager);
        sf::Vector2f relativeSize(imageXml.FloatAttribute("widthPercent"), imageXml.FloatAttribute("heightPercent"));
        sf::Vector2i texRect(baseSprite.getTextureRect().width, baseSprite.getTextureRect().height);

        auto sprite = std::unique_ptr<MenuSprite>(new MenuSprite(
                                                        baseSprite,
                                                        parsePosition(imageXml),
                                                        parseScale(imageXml),
                                                        id));

        auto toolTipText = imageXml.Attribute("tooltiptext");
        if(auto toolTipName = toolTipText ? imageXml.Attribute("tooltip") : nullptr)
        {
            auto tooltipIt = m_toolTips.find(toolTipName);
            if(tooltipIt == std::end(m_toolTips))
                throw std::runtime_error(utility::replace(utility::translateKey("@UnknownToolTip"), imageXml.Attribute("tooltip")));
            auto tooltip = tooltipIt->second;
            tooltip.setText(toolTipText);
            sprite->setToolTip(tooltip);
        }

        if(imageXml.Attribute("visibleWhen"))
            sprite->setVisibleWhenId(imageXml.IntAttribute("visibleWhen"));
        if(imageXml.Attribute("z"))
            sprite->setZLayer(imageXml.IntAttribute("z"));

        return sprite;
    });
}

std::vector<std::unique_ptr<InputBox>> MenuLoader::parseInputBox(
    const tinyxml2::XMLElement& menuXml)
{
    return parseElements<InputBox>("inputbox", menuXml,
        [&](const tinyxml2::XMLElement& inputBoxXml) -> std::unique_ptr<InputBox>
    {
        auto styleIt = m_inputBoxStyles.find(inputBoxXml.Attribute("style"));
        if(styleIt == std::end(m_inputBoxStyles))
            throw std::runtime_error(utility::replace(utility::translateKey("@UnknownInputBoxStyle"), inputBoxXml.Attribute("style")));
            
        auto style = styleIt->second;
        auto id = inputBoxXml.IntAttribute("id");
        auto inputLimit = inputBoxXml.IntAttribute("inputlimit");
        sf::Vector2f size(inputBoxXml.FloatAttribute("width"), inputBoxXml.FloatAttribute("height"));

        auto inputBox = std::unique_ptr<InputBox>(new InputBox(id, parsePosition(inputBoxXml), size, inputLimit, style));

        if(inputBoxXml.Attribute("visibleWhen"))
            inputBox->setVisibleWhenId(inputBoxXml.IntAttribute("visibleWhen"));
        if(inputBoxXml.Attribute("z"))
            inputBox->setZLayer(inputBoxXml.IntAttribute("z"));

        return inputBox;
    });
}

std::vector<std::unique_ptr<AnimationContainer>> MenuLoader::parseAnimationContainer(
        const tinyxml2::XMLElement& menuXml)
{
    return parseElements<AnimationContainer>("animationContainer", menuXml,
        [&](const tinyxml2::XMLElement& aniContainerXml) -> std::unique_ptr<AnimationContainer>
    {
        int id = aniContainerXml.IntAttribute("id");
        std::unique_ptr<AnimationContainer> animContainer(new AnimationContainer(id,
                                                                                 parsePosition(aniContainerXml),
                                                                                 parseScale(aniContainerXml),
                                                                                 _cloneHandler));
        if(auto animationsXml = aniContainerXml.FirstChildElement("animations"))
        {
            ProviderContext context(animContainer.get(), animContainer.get(), animContainer.get(), animContainer.get(), _cloneHandler);
            AnimationParser loader(AnimationContext(context, m_resourceManager, 0));
            auto animations = loader.parseMultiple(*animationsXml);
            for(auto ani = std::begin(animations); ani != std::end(animations); ++ani)
                animContainer->bindAnimation(std::move(*ani));
        }

        if(aniContainerXml.Attribute("visibleWhen"))
            animContainer->setVisibleWhenId(aniContainerXml.IntAttribute("visibleWhen"));
        if(aniContainerXml.Attribute("z"))
            animContainer->setZLayer(aniContainerXml.IntAttribute("z"));

        return animContainer;
    });
}

std::vector<std::unique_ptr<SubWindow>> MenuLoader::parseSubWindow(
    const tinyxml2::XMLElement& menuXml)
{
    return parseElements<SubWindow>("subwindow", menuXml,
        [&](const tinyxml2::XMLElement& subXml) -> std::unique_ptr<SubWindow>
    {
        auto id = subXml.IntAttribute("id");
        SubWindowStyle style;
        if(auto styleXml = subXml.FirstChildElement("style"))
        {
            if(auto sprite = styleXml->FirstChildElement("scrollTop"))
                style.scrollbarTop = getSprite(*sprite, m_resourceManager);

            if(auto sprite = styleXml->FirstChildElement("scrollMiddle"))
                style.scrollbarMiddle = getSprite(*sprite, m_resourceManager);

            if(auto sprite = styleXml->FirstChildElement("scrollBottom"))
                style.scrollbarBottom = getSprite(*sprite, m_resourceManager);
        }
        auto relativeSize = sf::Vector2f();
        subXml.QueryFloatAttribute("widthPercent", &relativeSize.x);
        subXml.QueryFloatAttribute("heightPercent", &relativeSize.y);
        auto size = sf::Vector2f(subXml.FloatAttribute("sizex"), subXml.FloatAttribute("sizey"));
        auto innerHeight = subXml.IntAttribute("innerheight");
        std::vector<std::unique_ptr<MenuElement>> subElements;
        addAll(parseButtons(subXml), subElements);
        addAll(parseBorders(subXml), subElements);
        addAll(parseCheckBoxes(subXml), subElements);
        addAll(parseCustomContents(subXml), subElements);
        addAll(parseSliders(subXml), subElements);
        addAll(parseLabels(subXml), subElements);
        addAll(parseImages(subXml), subElements);
        addAll(parseAnimationContainer(subXml), subElements);
        return std::unique_ptr<SubWindow>(new SubWindow(id, parsePosition(subXml), ScreenSize(size, relativeSize), innerHeight, subElements, style));
    });
}

void MenuLoader::parseButtonStyles(
    const tinyxml2::XMLElement& menuXml)
{
    std::unordered_map<std::string, ButtonStyle> buttonStyles;
    if(auto styles = menuXml.FirstChildElement("styles"))
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
            if(auto idle = styleXml->FirstChildElement("idle"))
                style.idleStyle = std::move(loadButtonStateStyle(*idle));

            if(auto hover = styleXml->FirstChildElement("hover"))
                style.hoverStyle = std::move(loadButtonStateStyle(*hover));

            if(auto pressed = styleXml->FirstChildElement("pressed"))
                style.pressedStyle = std::move(loadButtonStateStyle(*pressed));

            if(auto rect = styleXml->FirstChildElement("mouseRect"))
                style.mouseRect = sf::IntRect(
                    rect->IntAttribute("left"), rect->IntAttribute("top"),
                    rect->IntAttribute("width"), rect->IntAttribute("height"));

            buttonStyles.emplace(std::make_pair(name, std::move(style)));
        }
    }
    m_buttonStyles = std::move(buttonStyles);
}

void MenuLoader::parseBorderStyles(
    const tinyxml2::XMLElement& menuXml)
{
    std::unordered_map<std::string, BorderStyle> styles;
    
    if(auto stylesXml = menuXml.FirstChildElement("styles"))
    {
        tinyxml2::XMLDocument doc;
        std::string filename = resourcePath() + utility::toString("res/menus/") + stylesXml->Attribute("source");
        doc.LoadFile(filename.c_str());

        if(doc.Error())
        {
            doc.PrintError();
            throw std::runtime_error(utility::replace(utility::translateKey("@IncludeFileInvalid"), filename));
        }

        for(auto styleXml = doc.FirstChildElement("styles")->FirstChildElement("borderStyle");
            styleXml != nullptr; styleXml = styleXml->NextSiblingElement("borderStyle"))
        {
            if(auto name = styleXml->Attribute("name"))
            {
                BorderStyle style;
                parseStyle(*styleXml, style);
                styles[std::string(name)] = style;
            }
        }
    }
    m_borderStyles = std::move(styles);
}

void MenuLoader::parseStyle(const tinyxml2::XMLElement& borderXml, BorderStyle& style)
{
    for(auto background = borderXml.FirstChildElement("background");
        background != nullptr; background = background->NextSiblingElement("background"))
    {
        auto id = static_cast<BorderStyle::BackgroundId>(background->IntAttribute("id"));
        auto sprite = getSprite(*background, m_resourceManager);
        style.backgrounds[id] = sprite;
    }
    if(style.backgrounds.size() < 8)
        throw std::runtime_error(utility::replace(utility::translateKey("@InvalidBackground"), "Border"));
            
    for(auto deco = borderXml.FirstChildElement("deco");
        deco != nullptr; deco = deco->NextSiblingElement("deco"))
    {
        auto id = static_cast<BorderStyle::DecoId>(deco->IntAttribute("id") - 1);
        auto sprite = getSprite(*deco, m_resourceManager);
        sf::Vector2f offset;
        deco->QueryFloatAttribute("offsetx", &offset.x);
        deco->QueryFloatAttribute("offsety", &offset.y);
        style.decos[id].push_back(std::make_pair(sprite, offset));
    }

    if(auto innerOffsetsXml = borderXml.FirstChildElement("innerOffsets"))
    {
        innerOffsetsXml->QueryFloatAttribute("x", &style.innerOffsets.left);
        innerOffsetsXml->QueryFloatAttribute("y", &style.innerOffsets.top);
        innerOffsetsXml->QueryFloatAttribute("width", &style.innerOffsets.width);
        innerOffsetsXml->QueryFloatAttribute("height", &style.innerOffsets.height);
    }
    
    if(auto color = borderXml.Attribute("color"))
        style.color = utility::hexToColor(color);
}

ButtonStateStyle MenuLoader::loadButtonStateStyle(const tinyxml2::XMLElement& xml)
{
    ButtonStateStyle style;
    style.font = m_resourceManager.getBitmapFont(xml.Attribute("font"));
    style.textOffset = sf::Vector2f(xml.FloatAttribute("fontoffsetx"), xml.FloatAttribute("fontoffsety"));
    style.spriteOffset = sf::Vector2f(xml.FloatAttribute("offsetx"), xml.FloatAttribute("offsety"));
    style.sprite = getSprite(xml, m_resourceManager);
    if(auto soundName = xml.Attribute("sound"))
        style.sound = std::shared_ptr<SoundObject>(new SoundObject(soundName, m_resourceManager.getSoundManager()));

    if(auto animationsXml = xml.FirstChildElement("animations"))
    {
        animationsXml->QueryBoolAttribute("resetOnExit", &style.resetOnExit);
        style.animation = std::unique_ptr<AnimationContainer>(new AnimationContainer(0, ScreenLocation(), ScreenScale(), _cloneHandler));

        ProviderContext context(style.animation.get(), style.animation.get(), style.animation.get(), style.animation.get(), _cloneHandler);
        AnimationParser loader(AnimationContext(context, m_resourceManager, 0));
        auto animations = loader.parseMultiple(*animationsXml);
        for(auto it = begin(animations); it != end(animations); ++it)
            style.animation->bindAnimation(std::move(*it));
    }
    return style;
}

void MenuLoader::parseCheckBoxStyles(
    const tinyxml2::XMLElement& menuXml)
{
    std::unordered_map<std::string, CheckBoxStyle> checkboxStyles;
    if(auto styles = menuXml.FirstChildElement("styles"))
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
            
            if(auto unchecked = styleXml->FirstChildElement("unchecked"))
                style.uncheckedStyle = loadCheckBoxStateStyle(*unchecked);

            if(auto checked = styleXml->FirstChildElement("checked"))
                style.checkedStyle = loadCheckBoxStateStyle(*checked);

            if(auto hover = styleXml->FirstChildElement("hover"))
                style.hoverStyle = loadCheckBoxStateStyle(*hover);

            if(auto checkedHover = styleXml->FirstChildElement("checkedHover"))
                style.checkedHoverStyle = loadCheckBoxStateStyle(*checkedHover);
            else
                style.checkedHoverStyle = style.hoverStyle;

            if(auto rect = styleXml->FirstChildElement("mouseRect"))
                style.mouseRect = sf::IntRect(
                    rect->IntAttribute("left"), rect->IntAttribute("top"),
                    rect->IntAttribute("width"), rect->IntAttribute("height"));

            checkboxStyles[styleXml->Attribute("name")] = style;
        }
    }
    m_checkBoxStyles = std::move(checkboxStyles);
}

CheckBoxStateStyle MenuLoader::loadCheckBoxStateStyle(
    const tinyxml2::XMLElement& xml)
{
    CheckBoxStateStyle style;
    style.spriteOffset = sf::Vector2f(xml.FloatAttribute("offsetx"), xml.FloatAttribute("offsety"));
    style.sprite = getSprite(xml, m_resourceManager);
    return style;
}

void MenuLoader::parseSliderStyles(
    const tinyxml2::XMLElement& menuXml)
{
    std::unordered_map<std::string, SliderStyle> sliderStyles;
    if(auto styles = menuXml.FirstChildElement("styles"))
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
            
            if(auto idle = styleXml->FirstChildElement("idle"))
                style.idle = loadSliderStateStyle(*idle);

            if(auto active = styleXml->FirstChildElement("active"))
                style.active = loadSliderStateStyle(*active);
            
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
    m_sliderStyles = std::move(sliderStyles);
}

SliderStateStyle MenuLoader::loadSliderStateStyle(
    const tinyxml2::XMLElement& xml)
{
    SliderStateStyle style;
    style.backgroundOffset = sf::Vector2f(xml.FloatAttribute("backgroundoffsetx"), xml.FloatAttribute("backgroundoffsety"));
    style.background = getSprite("background", xml, m_resourceManager);

    style.buttonOffset = sf::Vector2f(xml.FloatAttribute("slideroffsetx"), xml.FloatAttribute("slideroffsety"));
    style.button = getSprite("slider", xml, m_resourceManager);
    return style;
}

void MenuLoader::parseToolTipStyles(
    const tinyxml2::XMLElement& menuXml)
{
    std::unordered_map<std::string, ToolTip> toolTip;
    if(auto styles = menuXml.FirstChildElement("styles"))
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
                texture = getSprite(*background, m_resourceManager).getSprite();
                backgroundMap[id] = texture;
                ++counter;
            }
            if(counter < 9)
                throw std::runtime_error(utility::replace(utility::translateKey("@InvalidBackground"), "ToolTip"));

            if(auto textXml = tooltipXml->FirstChildElement("text"))
            {
                if(auto fontName = textXml->Attribute("font"))
                {  
                    ToolTip tempToolTip("",
                                        m_resourceManager.getBitmapFont(fontName),
                                        sf::Vector2f(textXml->FloatAttribute("offsetx"),
                                                     textXml->FloatAttribute("offsety")),
                                        sf::Vector2f(tooltipXml->FloatAttribute("offsetx"), 
                                                     tooltipXml->FloatAttribute("offsety")),
                                        backgroundMap);
            
                    toolTip[tooltipXml->Attribute("name")] = tempToolTip;
                }
            }
        }
    }
    m_toolTips = std::move(toolTip);
}

void MenuLoader::parseInputBoxStyles(
    const tinyxml2::XMLElement& menuXml)
{
    std::unordered_map<std::string, InputBoxStyle> inputBoxStyle;
    if(auto styles = menuXml.FirstChildElement("styles"))
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
            if(auto textXml = inputBoxStyleXml->FirstChildElement("text"))
            {
                if(auto fontName = textXml->Attribute("font"))
                    inputBoxStyle[styleName].font = m_resourceManager.getBitmapFont(fontName);
                inputBoxStyle[styleName].textOffset = sf::Vector2f(static_cast<float>(textXml->IntAttribute("offsetx")),
                                                                   static_cast<float>(textXml->IntAttribute("offsety")));
            }
            int id = 0;
            int counter = 0;
            for(auto background = inputBoxStyleXml->FirstChildElement("background");
                background != nullptr; background = background->NextSiblingElement("background"))
            {
                id = background->IntAttribute("id");
                inputBoxStyle[styleName].background[id] = getSprite(*background, m_resourceManager);
                ++counter;
            }

            if(counter < 9)
                throw std::runtime_error(utility::replace(utility::translateKey("@InvalidBackground"), "InputBox"));

            if(auto caret = inputBoxStyleXml->FirstChildElement("caret"))
            {
                inputBoxStyle[styleName].caret = getSprite(*caret, m_resourceManager);
                inputBoxStyle[styleName].caretOffset = sf::Vector2f(caret->FloatAttribute("offsetX"), caret->FloatAttribute("offsetY"));
                inputBoxStyle[styleName].caretBlinkFrequency = caret->FloatAttribute("frequency");
            }
            else
                throw std::runtime_error(utility::translateKey("@NoCaret"));
        }
    }

    m_inputBoxStyles = std::move(inputBoxStyle);
}
