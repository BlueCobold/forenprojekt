
#include "AnimationParser.hpp"

#include "ControllerParser.hpp"
#include "ResourceManager.hpp"
#include "SpriteSheet.hpp"
#include "ValueParser.hpp"
#include "../animation/Animation.hpp"
#include "../animation/CloneHandler.hpp"
#include "../animation/OrientedObject.hpp"
#include "../animation/Stoppable.hpp"
#include "../animation/TimedObject.hpp"
#include "../animation/VariableHandler.hpp"
#include "../animation/provider/ValueProvider.hpp"
#include "../rendering/Shader.hpp"
#include "../Utility.hpp"

std::vector<std::unique_ptr<Animation>> AnimationParser::parseMultiple(const tinyxml2::XMLElement& xml) const
{
    std::vector<std::unique_ptr<Animation>> animations;
    for(auto anim = xml.FirstChildElement("animation"); anim != nullptr; anim = anim->NextSiblingElement("animation"))
    {
        auto singleAnims = parseSingle(*anim);
        for(auto ani = begin(singleAnims); ani != end(singleAnims); ++ani)
            animations.push_back(std::move(*ani));
    }

    return animations;
}

std::vector<std::unique_ptr<Animation>> AnimationParser::parseSingle(const tinyxml2::XMLElement& xml) const
{
    std::vector<std::unique_ptr<Animation>> animations;

    int copies = 1;
    xml.QueryIntAttribute("copies", &copies);
    if(copies > 0)
    {
        std::unique_ptr<Animation> animation = parseSingleTag(xml);
        auto original = animation.get();
        for(int copy = 0; copy < copies; copy++)
        {
            std::unique_ptr<Animation> ani = copy > 0 ? original->clone() : std::move(animation);
            ani->setValueOf("cloneId", static_cast<float>(copy));
        
            if(m_callback != nullptr)
                m_callback(ani, xml);

            if(ani != nullptr)
                animations.push_back(std::move(ani));
        }
    }
    return animations;
}

std::unique_ptr<Animation> AnimationParser::parseSingleTag(const tinyxml2::XMLElement& xml) const
{
    int frames = 1;
    if(auto frameIndex = xml.FirstChildElement("frameindex"))
         frameIndex->QueryIntAttribute("frames", &frames);

    const sf::Texture* texture = nullptr;
    bool bindOnUsage = false;
    const SpriteSheet* sheet = nullptr;
    SpriteSheet::SpriteData sprite;
    if(auto textureName = xml.Attribute("texture"))
    {
        texture = m_context.resourceManager.getTexture(textureName);
        std::string prefix("offscreenBuffer");
        // is this an offscreen texture name?
        if (!std::string(textureName).compare(0, prefix.size(), prefix))
            bindOnUsage = true;
    }
    else
    {
        auto sheetName = xml.Attribute("spritesheet");
        if (!sheetName)
            sheetName = xml.Attribute("spriteSheet");
        auto spriteName = xml.Attribute("sprite");
        if(sheetName && spriteName && (sheet = m_context.resourceManager.getSpriteSheet(sheetName)) != nullptr)
        {
            texture = m_context.resourceManager.getTexture(sheet->getTextureName());
            sprite = sheet->get(spriteName);
        }
    }
    if(texture == nullptr)
        throw std::runtime_error(utility::translateKey("NoTextureFound"));

    int width = -1, height = -1;
    if(sheet != nullptr)
    {
        width = sprite.width;
        height = sprite.height;
    }
    xml.QueryIntAttribute("width", &width);
    xml.QueryIntAttribute("height", &height);

    bool rotate = false;
    xml.QueryBoolAttribute("rotate", &rotate);
    sf::Vector2f offset;
    xml.QueryFloatAttribute("x", &offset.x);
    xml.QueryFloatAttribute("y", &offset.y);
    sf::Vector2f origin;
    if(sheet != nullptr)
    {
        origin.x = sprite.originX;
        origin.y = sprite.originY;
    }
    xml.QueryFloatAttribute("midx", &origin.x);
    xml.QueryFloatAttribute("midy", &origin.y);

    bool horizontal = true;
    if(auto alignment = xml.Attribute("alignment"))
        horizontal = std::string("vertical") != alignment;

    std::unique_ptr<Animation> anim(new Animation(frames, width, height, rotate, origin, offset, horizontal));
    auto aniContext = ProviderContext(m_context.providerContext).withCustomTag("stopAnimation", anim.get());
    if(aniContext.variableHandler == nullptr)
        aniContext.variableHandler = anim.get();
    ProviderParser providerParser(aniContext);

    if(xml.Attribute("bufferId"))
        anim->setBufferId(xml.IntAttribute("bufferId"));
    if(xml.Attribute("z"))
        anim->setDrawOrder(xml.IntAttribute("z"));
    if(xml.Attribute("alignToView"))
        anim->alignToView(xml.BoolAttribute("alignToView"));
    if(xml.Attribute("scaleToScreenSize"))
        anim->scaleToScreenSize(xml.BoolAttribute("scaleToScreenSize"));

    anim->bindCloneHandler(aniContext.cloneHandler);
    if(auto shaderName = xml.Attribute("shader"))
    {
        if(Shader::isUsable())
            anim->bindShader(*m_context.resourceManager.getShader(shaderName));
        else
            return nullptr; // no shaders available -> skip animation
    }
    else if(xml.Attribute("whenShaders") && xml.BoolAttribute("whenShaders") != Shader::isUsable())
        return nullptr; // this animation requires an explicit shader-availability to decide if to draw or not

    if(auto stencil = xml.FirstChildElement("stencil"))
    {
        if(auto op = stencil->Attribute("op"))
        {
            if(std::string(op) == "write")
                anim->setStencilInfo(StencilInfo(StencilInfo::Write, stencil->IntAttribute("ref"), stencil->IntAttribute("mask")));
            else if(std::string(op) == "test")
                anim->setStencilInfo(StencilInfo(StencilInfo::Test, stencil->IntAttribute("ref"), stencil->IntAttribute("mask")));
        }
    }
    anim->setStopOnAlphaZero(xml.BoolAttribute("stopOnAlphaZero"));

    if(auto frameIndex = xml.FirstChildElement("frameindex"))
    {
        auto child = frameIndex->FirstChildElement();
        if(child == nullptr)
            throw std::runtime_error(utility::translateKey("NoProvider"));
        anim->bindFrameProvider(providerParser.parseSingle(*child));
    }
    else
        anim->bindFrameProvider(std::unique_ptr<ValueProvider>(new StaticProvider(0)));

    sf::Vector2f sourceOffset;
    if(sheet != nullptr)
    {
        sourceOffset.x = static_cast<float>(sprite.x);
        sourceOffset.y = static_cast<float>(sprite.y);
    }
    xml.QueryFloatAttribute("srcx", &sourceOffset.x);
    xml.QueryFloatAttribute("srcy", &sourceOffset.y);

    anim->bindTexture(*texture, sourceOffset, bindOnUsage);
    
    ControllerParser controllerParser(aniContext);
    auto position = controllerParser.parsePosition(xml);
    anim->bindPositionController(std::move(*position[0]), std::move(*position[1]));

    auto scale = controllerParser.parseScale(xml);
    anim->bindScaleController(std::move(*scale[0]), std::move(*scale[1]));

    auto color = controllerParser.parseColor(xml);
    anim->bindColorController(std::move(*color[0]), std::move(*color[1]), std::move(*color[2]), std::move(*color[3]));

    anim->bindRotationController(controllerParser.parseRotation(xml));
    
    if(anim->getBufferId() == UINT_MAX)
        anim->setBufferId(m_context.defaultTargetBuffer);

    if(auto constants = xml.FirstChildElement("constants"))
        ValueParser::parseConstants(*constants, *anim.get());

    if(auto layout = xml.FirstChildElement("layout"))
    {
        if(auto sprites = layout->FirstChildElement("sprites"))
        {
            if(sheet == nullptr)
                throw std::runtime_error(utility::translateKey("CannotUseSpriteLayout"));

            std::vector<sf::Vector2i> srcPos, sizes, origins, srcOffset;
            ValueParser::parseSpriteValueList(*sprites, *sheet, srcPos, srcOffset, sizes, origins);
            anim->setLayout(srcPos, srcOffset, sizes, origins);
        }
        else
        {
            std::vector<sf::Vector2i> srcoffsets = ValueParser::parseValueList(*layout, "srcx", "srcy");
            std::vector<sf::Vector2i> sizes = ValueParser::parseValueList(*layout, "width", "height");
            std::vector<sf::Vector2i> origins = ValueParser::parseValueList(*layout, "midx", "midy");
            std::vector<sf::Vector2i> offsets = ValueParser::parseValueList(*layout, "xoffset", "yoffset");
            anim->setLayout(srcoffsets, offsets, sizes, origins);
        }
    }

    if(auto blend = xml.Attribute("blending"))
    {
        auto mode = Blending::RegularAlpha;
        if(std::string("add") == blend)
            mode = Blending::Add;
        else if(std::string("mul") == blend)
            mode = Blending::Multiply;
        else if(std::string("premul") == blend)
            mode = Blending::PreMultipliedAlpha;
        anim->setBlending(mode);
    }

    return anim;
}
