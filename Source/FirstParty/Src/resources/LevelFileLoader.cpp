#include "LevelFileLoader.hpp"

#include "../animation/provider/Absolute.hpp"
#include "../animation/provider/Adder.hpp"
#include "../animation/provider/AngleProvider.hpp"
#include "../animation/provider/CachedProvider.hpp"
#include "../animation/provider/Clamp.hpp"
#include "../animation/provider/Count.hpp"
#include "../animation/provider/Delay.hpp"
#include "../animation/provider/FloatToInt.hpp"
#include "../animation/provider/IfPositive.hpp"
#include "../animation/provider/Inverse.hpp"
#include "../animation/provider/Maximum.hpp"
#include "../animation/provider/Minimum.hpp"
#include "../animation/provider/Modulo.hpp"
#include "../animation/provider/MouseProvider.hpp"
#include "../animation/provider/Multiplier.hpp"
#include "../animation/provider/Negate.hpp"
#include "../animation/provider/Nop.hpp"
#include "../animation/provider/Pow.hpp"
#include "../animation/provider/Ramp.hpp"
#include "../animation/provider/RandomProvider.hpp"
#include "../animation/provider/SetVariable.hpp"
#include "../animation/provider/Sine.hpp"
#include "../animation/provider/StaticProvider.hpp"
#include "../animation/provider/Step.hpp"
#include "../animation/provider/Stop.hpp"
#include "../animation/provider/Switch.hpp"
#include "../animation/provider/Substractor.hpp"
#include "../animation/provider/TimeProvider.hpp"
#include "../animation/provider/KeyProvider.hpp"
#include "../animation/provider/VariableProvider.hpp"
#include "../rendering/Shader.hpp"
#include "../resources/SpriteSheet.hpp"

#include <map>
#include <vector>

void LevelFileLoader::parseConstants(const tinyxml2::XMLElement* xml,
    VariableHandler* holder)
{
    std::map<std::string, std::unique_ptr<ValueProvider>> map;
    for(auto var = xml->FirstAttribute(); var != nullptr; var = var->Next())
        holder->setValueOf(var->Name(), var->FloatValue());
}

std::vector<sf::Vector2i> parseValueList(const tinyxml2::XMLElement* xml, const std::string& x, const std::string& y)
{
    std::vector<sf::Vector2i> result;
    auto xxml = xml->FirstChildElement(x.c_str());
    auto yxml = xml->FirstChildElement(y.c_str());
    if(!xxml || !yxml)
        return result;

    std::istringstream tokens(xxml->GetText());
    std::string token;
    while(std::getline(tokens, token, ','))
        result.push_back(sf::Vector2i(utility::stringTo<int>(token), 0));

    tokens.str(yxml->GetText());
    size_t i = 0;
    while(std::getline(tokens, token, ','))
    {
        if(i > result.size())
            throw std::runtime_error(utility::replace(utility::translateKey("InvalidLayout"), y));
        result[i].y = utility::stringTo<int>(token);
        ++i;
    }
    if(i != result.size())
        throw std::runtime_error(utility::replace(utility::translateKey("InvalidLayout"), y));
    return result;
}

void parseSpriteValueList(const tinyxml2::XMLElement* xml,
    const SpriteSheet* sheet,
    std::vector<sf::Vector2i>& src,
    std::vector<sf::Vector2i>& offset,
    std::vector<sf::Vector2i>& sizes,
    std::vector<sf::Vector2i>& origins)
{
    std::istringstream tokens(xml->GetText());
    std::string token;
    while(std::getline(tokens, token, ','))
    {
        std::stringstream trimmer;
        trimmer << token;
        token.clear();
        trimmer >> token;
        auto sprite = sheet->get(token);
        src.push_back(sf::Vector2i(sprite.x, sprite.y));
        sizes.push_back(sf::Vector2i(sprite.width, sprite.height));
        offset.push_back(sf::Vector2i(sprite.offsetX, sprite.offsetY));
        origins.push_back(sf::Vector2i(static_cast<int>(sprite.originX), static_cast<int>(sprite.originY)));
    }
}

void LevelFileLoader::parseColorController(
    Animation* animation,
    const tinyxml2::XMLElement* xml,
    AnimatedGraphics* animated,
    VariableHandler* handler,
    std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
    CloneHandler& cloneHandler)
{
    std::unique_ptr<ValueProvider> red, green, blue, alpha, tmp;
    auto static_color_element = xml->FirstChildElement("color");
    const char* value = nullptr;
    while(static_color_element && !(value = static_color_element->Attribute("value")))
        static_color_element = static_color_element->NextSiblingElement("color");

    if(static_color_element && value)
    {
        sf::Color col = utility::hexToColor(value);
        red = std::unique_ptr<ValueProvider>(new StaticProvider(col.r / 255.f));
        green = std::unique_ptr<ValueProvider>(new StaticProvider(col.g / 255.f));
        blue = std::unique_ptr<ValueProvider>(new StaticProvider(col.b / 255.f));
        alpha = std::unique_ptr<ValueProvider>(new StaticProvider(col.a / 255.f));
    }

    tmp = findController(xml, animated, handler, animation, "color", "channel", "red", functions, cloneHandler);
    if(tmp != nullptr)
        red = std::move(tmp);
    tmp = findController(xml, animated, handler, animation, "color", "channel", "green", functions, cloneHandler);
    if(tmp != nullptr)
        green = std::move(tmp);
    tmp = findController(xml, animated, handler, animation, "color", "channel", "blue", functions, cloneHandler);
    if(tmp != nullptr)
        blue = std::move(tmp);
    tmp = findController(xml, animated, handler, animation, "color", "channel", "alpha", functions, cloneHandler);
    if(tmp != nullptr)
        alpha = std::move(tmp);

    animation->bindColorController(std::move(red), std::move(green), std::move(blue), std::move(alpha));
}

void LevelFileLoader::parsePositionController(
    Animation* animation,
    const tinyxml2::XMLElement* xml,
    AnimatedGraphics* animated,
    VariableHandler* handler,
    std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
    CloneHandler& cloneHandler)
{
    std::unique_ptr<ValueProvider> xProvider = findController(xml, animated, handler, animation, "position", "axis", "x", functions, cloneHandler);
    std::unique_ptr<ValueProvider> yProvider = findController(xml, animated, handler, animation, "position", "axis", "y", functions, cloneHandler);
    animation->bindPositionController(std::move(xProvider), std::move(yProvider));
}

void LevelFileLoader::parseScaleController(
    Animation* animation,
    const tinyxml2::XMLElement* xml,
    AnimatedGraphics* animated,
    VariableHandler* handler,
    std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
    CloneHandler& cloneHandler)
{
    std::unique_ptr<ValueProvider> xScaleProvider = findController(xml, animated, handler, animation, "scale", "axis", "x", functions, cloneHandler);
    std::unique_ptr<ValueProvider> yScaleProvider = findController(xml, animated, handler, animation, "scale", "axis", "y", functions, cloneHandler);
    animation->bindScaleController(std::move(xScaleProvider), std::move(yScaleProvider));
}

void LevelFileLoader::parseRotationController(
    Animation* animation,
    const tinyxml2::XMLElement* xml,
    AnimatedGraphics* animated,
    VariableHandler* handler,
    std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
    CloneHandler& cloneHandler)
{
    auto rotation = xml->FirstChildElement("rotation");
    if(rotation != nullptr && rotation->FirstChildElement() != nullptr)
    {
        std::unique_ptr<ValueProvider> rotProvider = parseProvider(rotation->FirstChildElement(), animated, handler, animation, functions, cloneHandler);
        animation->bindRotationController(std::move(rotProvider));
    }
}

std::unique_ptr<Animation> LevelFileLoader::parseAnimation(
    const tinyxml2::XMLElement* xml,
    AnimatedGraphics* animated,
    VariableHandler* handler,
    ResourceManager& resourceManager,
    std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
    CloneHandler& cloneHandler)
{
    auto frameIndex = xml->FirstChildElement("frameindex");
    int frames = 1;
    if(frameIndex != nullptr)
        frames = frameIndex->IntAttribute("frames");
    const sf::Texture* texture = nullptr;
    bool bindOnUsage = false;
    const SpriteSheet* sheet = nullptr;
    SpriteSheet::SpriteData sprite;
    if(auto textureName = xml->Attribute("texture"))
    {
        texture = resourceManager.getTexture(textureName);
        std::string prefix("offscreenBuffer");
        // is this an offscreen texture name?
        if (!std::string(textureName).compare(0, prefix.size(), prefix))
            bindOnUsage = true;
    }
    else
    {
        auto sheetName = xml->Attribute("spritesheet");
        auto spriteName = xml->Attribute("sprite");
        if(sheetName && spriteName && (sheet = resourceManager.getSpriteSheet(sheetName)) != nullptr)
        {
            texture = resourceManager.getTexture(sheet->getTextureName());
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
    if(xml->Attribute("width") != nullptr)
        width = xml->IntAttribute("width");
    if(xml->Attribute("height") != nullptr)
        height = xml->IntAttribute("height");

    bool rotate = xml->BoolAttribute("rotate");
    bool scaleToScreenSize = xml->BoolAttribute("scaleToScreenSize");
    sf::Vector2f offset;
    offset.x = xml->FloatAttribute("x");
    offset.y = xml->FloatAttribute("y");
    sf::Vector2f origin;
    if(sheet != nullptr)
    {
        origin.x = sprite.originX;
        origin.y = sprite.originY;
    }
    if(xml->Attribute("midx"))
        origin.x = xml->FloatAttribute("midx");
    if(xml->Attribute("midy"))
        origin.y = xml->FloatAttribute("midy");

    bool horizontal = true;
    if(xml->Attribute("alignment"))
        horizontal = std::string("vertical") != xml->Attribute("alignment");

    std::unique_ptr<Animation> anim(new Animation(frames, width, height, rotate, origin, offset, horizontal));
    if(handler == nullptr)
        handler = anim.get();

    if(xml->Attribute("bufferId"))
        anim->setBufferId(xml->IntAttribute("bufferId"));
    if(xml->Attribute("z"))
        anim->setDrawOrder(xml->IntAttribute("z"));
    if(xml->Attribute("alignToView"))
        anim->alignToView(xml->BoolAttribute("alignToView"));
    if(xml->Attribute("scaleToScreenSize"))
        anim->scaleToScreenSize(true);

    anim->bindCloneHandler(cloneHandler);
    if(auto shaderName = xml->Attribute("shader"))
    {
        if(Shader::isUsable())
            anim->bindShader(*resourceManager.getShader(shaderName));
        else
            return nullptr; // no shaders available -> skip animation
    }
    else if(xml->Attribute("whenShaders") && xml->BoolAttribute("whenShaders") != Shader::isUsable())
        return nullptr; // this animation requires an explicit shader-availability to decide if to draw or not

    if(auto stencil = xml->FirstChildElement("stencil"))
    {
        if(auto op = stencil->Attribute("op"))
        {
            if(std::string(op) == "write")
                anim->setStencilInfo(StencilInfo(StencilInfo::Write, stencil->IntAttribute("ref"), stencil->IntAttribute("mask")));
            if(std::string(op) == "test")
                anim->setStencilInfo(StencilInfo(StencilInfo::Test, stencil->IntAttribute("ref"), stencil->IntAttribute("mask")));
        }
    }
    anim->setStopOnAlphaZero(xml->BoolAttribute("stopOnAlphaZero"));

    std::unique_ptr<ValueProvider> provider;
    if(frameIndex == nullptr)
        provider.reset(new StaticProvider(0));
    else
    {
        auto child = frameIndex->FirstChildElement();
        if(child == nullptr)
            throw std::runtime_error(utility::translateKey("NoProvider"));
        provider = std::move(parseProvider(child, animated, handler, anim.get(), functions, cloneHandler));
    }
    anim->bindFrameProvider(std::move(provider));

    sf::Vector2f sourceOffset;
    if(sheet != nullptr)
    {
        sourceOffset.x = static_cast<float>(sprite.x);
        sourceOffset.y = static_cast<float>(sprite.y);
    }
    if(xml->Attribute("srcx"))
        sourceOffset.x = xml->FloatAttribute("srcx");
    if(xml->Attribute("srcy"))
        sourceOffset.y = xml->FloatAttribute("srcy");

    anim->bindTexture(*texture, sourceOffset, bindOnUsage);

    std::vector<std::unique_ptr<Animation>> subAnimations;

    parsePositionController(anim.get(), xml, animated, handler, functions, cloneHandler);
    parseScaleController(anim.get(), xml, animated, handler, functions, cloneHandler);
    parseColorController(anim.get(), xml, animated, handler, functions, cloneHandler);
    parseRotationController(anim.get(), xml, animated, handler, functions, cloneHandler);
    if(auto constants = xml->FirstChildElement("constants"))
        parseConstants(constants, anim.get());

    if(auto layout = xml->FirstChildElement("layout"))
    {
        if(auto sprites = layout->FirstChildElement("sprites"))
        {
            if(sheet == nullptr)
                throw std::runtime_error(utility::translateKey("CannotUseSpriteLayout"));
            std::vector<sf::Vector2i> srcPos, sizes, origins, srcOffset;
            parseSpriteValueList(sprites, sheet, srcPos, srcOffset, sizes, origins);
            anim->setLayout(srcPos, srcOffset, sizes, origins);
        }
        else
        {
            std::vector<sf::Vector2i> srcoffsets = parseValueList(layout, "srcx", "srcy");
            std::vector<sf::Vector2i> sizes = parseValueList(layout, "width", "height");
            std::vector<sf::Vector2i> origins = parseValueList(layout, "midx", "midy");
            std::vector<sf::Vector2i> offsets = parseValueList(layout, "xoffset", "yoffset");
            anim->setLayout(srcoffsets, offsets, sizes, origins);
        }
    }

    if(auto blend = xml->Attribute("blending"))
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

std::vector<std::unique_ptr<ValueProvider>> LevelFileLoader::parseProviders(const tinyxml2::XMLElement* xml,
    AnimatedObject* animated,
    VariableHandler* handler,
    Stoppable* stoppable,
    std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
    CloneHandler& cloneHandler)
{
    std::vector<std::unique_ptr<ValueProvider>> providers;
    for(auto child = xml->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        if(std::string(child->Name())=="function")
        {
            if(functions == nullptr)
                throw std::runtime_error(utility::translateKey("FunctionNull"));
            auto function = functions->find(child->Attribute("name"));
            if(function == end(*functions))
                throw std::runtime_error(utility::replace(utility::translateKey("NoTemplate"), child->Attribute("name")));
            auto subs = parseProviders(function->second, animated, handler, stoppable, functions, cloneHandler);
            for(auto sub = begin(subs); sub != end(subs); ++sub)
                providers.push_back(std::move(*sub));
        }
        else
        {
            std::unique_ptr<ValueProvider> provider = parseProvider(child, animated, handler, stoppable, functions, cloneHandler);
            if(provider != nullptr)
                providers.push_back(std::move(provider));
        }
    }
    return providers;
}

std::vector<float> parseFloatList(const std::string& valueString)
{
    std::vector<float> results;
    std::stringstream ss(valueString);
    std::string item;
    while(std::getline(ss, item, ','))
        results.push_back(utility::stringTo<float>(item));
    return results;
}

std::unique_ptr<ValueProvider> LevelFileLoader::parseProvider(const tinyxml2::XMLElement* xml,
    AnimatedObject* animated,
    VariableHandler* handler,
    Stoppable* stoppable,
    std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
    CloneHandler& cloneHandler)
{
    if(std::string(xml->Name())=="time")
        return std::unique_ptr<TimeProvider>(new TimeProvider(*animated, cloneHandler.createTimeProviderCloneHandler()));
    else if(std::string(xml->Name())=="stop")
        return std::unique_ptr<Stop>(new Stop(*animated, cloneHandler.createStopProviderCloneHandler()));
    else if(std::string(xml->Name())=="stopAnimation")
    {
        if(stoppable != nullptr)
            return std::unique_ptr<Stop>(new Stop(*stoppable, cloneHandler.createStopProviderCloneHandler()));
        throw std::runtime_error("<stopAnimation> has been used on an invalid element");
    }
    else if(std::string(xml->Name())=="angle")
        return std::unique_ptr<AngleProvider>(new AngleProvider(*animated, cloneHandler.createAngleProviderCloneHandler()));
    else if(std::string(xml->Name())=="static")
        return std::unique_ptr<StaticProvider>(new StaticProvider(xml->FloatAttribute("value")));
    else if(std::string(xml->Name())=="var" || std::string(xml->Name())=="variable")
        return std::unique_ptr<VariableProvider>(new VariableProvider(*handler, xml->Attribute("name"),
                                                                      cloneHandler.createVariableProviderCloneHandler()));
    else if(std::string(xml->Name())=="count")
        return std::unique_ptr<Count>(new Count(xml->FloatAttribute("start"), xml->FloatAttribute("increment")));
    else if(std::string(xml->Name())=="random")
        return std::unique_ptr<RandomProvider>(new RandomProvider(xml->FloatAttribute("min"), xml->FloatAttribute("max")));
    else if(std::string(xml->Name())=="keyDown")
        return std::unique_ptr<KeyProvider>(new KeyProvider((int)(xml->Attribute("key")[0])));
    else if(std::string(xml->Name())=="mouse")
        return std::unique_ptr<MouseProvider>(new MouseProvider(xml->Attribute("axis") ? std::string(xml->Attribute("axis")) == "x" : true));
    else if(std::string(xml->Name())=="function")
    {
        if(functions == nullptr)
            throw std::runtime_error(utility::translateKey("FunctionNull"));

        auto funcName = xml->Attribute("name");
        if(funcName == nullptr)
            throw std::runtime_error(utility::translateKey("NoName"));

        auto function = functions->find(funcName);
        if(function == end(*functions))
            throw std::runtime_error(utility::replace(utility::translateKey("NoTemplate"), funcName));

        auto providers = parseProviders(function->second, animated, handler, stoppable, functions, cloneHandler);
        if(providers.size()>1)
            throw std::runtime_error(utility::replace(utility::translateKey("TemplateChild"), funcName));

        return std::move(providers[0]);
    }
    else
    {
        auto providers = parseProviders(xml, animated, handler, stoppable, functions, cloneHandler);
        if(std::string(xml->Name())=="setVar" || std::string(xml->Name())=="setVariable")
            return std::unique_ptr<SetVariable>(new SetVariable(*handler, xml->Attribute("name"), std::move(providers[0]), xml->BoolAttribute("print"),
                                                                cloneHandler.createSetVariableProviderCloneHandler()));
        else if(std::string(xml->Name())=="abs")
            return std::unique_ptr<Absolute>(new Absolute(std::move(providers[0])));
        else if(std::string(xml->Name())=="sine")
            return std::unique_ptr<Sine>(new Sine(std::move(providers[0])));
        else if(std::string(xml->Name())=="cache")
            return std::unique_ptr<CachedProvider>(new CachedProvider(std::move(providers[0]),
                xml->Attribute("reset") ? std::string("true") != xml->Attribute("reset") : false));
        else if(std::string(xml->Name())=="int")
            return std::unique_ptr<FloatToInt>(new FloatToInt(std::move(providers[0])));
        else if(std::string(xml->Name())=="add")
            return std::unique_ptr<Adder>(new Adder(std::move(providers)));
        else if(std::string(xml->Name())=="ifpositive")
            return std::unique_ptr<IfPositive>(new IfPositive(std::move(providers)));
        else if(std::string(xml->Name())=="mul")
            return std::unique_ptr<Multiplier>(new Multiplier(std::move(providers)));
        else if(std::string(xml->Name())=="min")
            return std::unique_ptr<Minimum>(new Minimum(std::move(providers)));
        else if(std::string(xml->Name())=="max")
            return std::unique_ptr<Maximum>(new Maximum(std::move(providers)));
        else if(std::string(xml->Name())=="mod")
            return std::unique_ptr<Modulo>(new Modulo(std::move(providers)));
        else if(std::string(xml->Name())=="pow")
            return std::unique_ptr<Pow>(new Pow(std::move(providers)));
        else if(std::string(xml->Name())=="nop")
            return std::unique_ptr<Nop>(new Nop(std::move(providers)));
        else if(std::string(xml->Name())=="clamp")
            return std::unique_ptr<Clamp>(new Clamp(std::move(providers)));
        else if(std::string(xml->Name())=="step")
        {
            float threshold = 1;
            xml->QueryFloatAttribute("threshold", &threshold);
            return std::unique_ptr<Step>(new Step(std::move(providers), threshold));
        }
        else if(std::string(xml->Name())=="sub")
            return std::unique_ptr<Substractor>(new Substractor(std::move(providers)));
        else if(std::string(xml->Name())=="switch")
            return std::unique_ptr<Switch>(new Switch(std::move(providers), parseFloatList(xml->Attribute("cases"))));
        else if(std::string(xml->Name())=="neg")
            return std::unique_ptr<Negate>(new Negate(std::move(providers[0])));
        else if(std::string(xml->Name())=="ramp")
            return std::unique_ptr<Ramp>(new Ramp(xml->FloatAttribute("min"), xml->FloatAttribute("max"),
                                                  std::move(providers[0])));
        else if(std::string(xml->Name())=="delay")
            return std::unique_ptr<Delay>(new Delay(xml->FloatAttribute("start"), xml->FloatAttribute("duration"),
                                                  std::move(providers[0])));
        else if(std::string(xml->Name())=="inv")
            return std::unique_ptr<Inverse>(new Inverse(std::move(providers[0])));
    }
    throw std::runtime_error(utility::replace(utility::translateKey("Unknown"), xml->Name()));
}

std::unordered_map<std::string, const tinyxml2::XMLElement*> LevelFileLoader::parseList(
     const tinyxml2::XMLElement* xml,
     const std::string& attributeName,
     const std::string& key)
{
    std::unordered_map<std::string, const tinyxml2::XMLElement*> tempList;

    for(auto iterator = xml->FirstChildElement(attributeName.c_str());
       iterator != nullptr; iterator = iterator->NextSiblingElement(attributeName.c_str()))
    {
        if(auto nodeName = iterator->Attribute(key.c_str()))
            tempList.insert(std::make_pair(std::string(nodeName), iterator));
    }

    return std::move(tempList);
}

std::vector<std::string> LevelFileLoader::parseGrid(const tinyxml2::XMLElement* xml)
{
    std::string data = xml->GetText();

    // Remove first and last line breaks
    data = data.substr(data.find_first_of('\n')+1, data.find_last_of('\n'));

    std::vector<std::string> lines;

    // Get the lines separated without the newline char
    for(std::size_t i = 0; i < data.length(); i = data.find('\n', i)+1)
        lines.push_back(data.substr(i, data.find('\n', i)-i));

    return std::move(lines);
}

void LevelFileLoader::parseBodyDef(const tinyxml2::XMLElement* physicXml,
    Entity* entity,
    VariableHandler* handler,
    std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
    b2BodyDef& bodyDef,
    const sf::Vector2u& position,
    CloneHandler& cloneHandler)
{
    auto bodyXml = physicXml->FirstChildElement("body");
    if(std::string(bodyXml->Attribute("type")) == "static")
        bodyDef.type = b2_staticBody;
    else if(std::string(bodyXml->Attribute("type")) == "kinematic")
        bodyDef.type = b2_kinematicBody;
    else if(std::string(bodyXml->Attribute("type")) == "dynamic")
        bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(static_cast<float>(utility::toMeter(position.x)), static_cast<float>(utility::toMeter(position.y)));
    bodyDef.angle = utility::toRadian(bodyXml->FloatAttribute("angle"));
    bodyDef.fixedRotation = bodyXml->BoolAttribute("fixedRotation");
    bodyDef.angularDamping = bodyXml->BoolAttribute("angularDamping");
    parseKinematics(bodyXml, entity, handler, functions, cloneHandler);
}

void LevelFileLoader::parseKinematics(const tinyxml2::XMLElement* element,
    Entity* entity,
    VariableHandler* handler,
    std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
    CloneHandler& cloneHandler)
{
    auto kinematics = element->FirstChildElement("kinematics");
    if(kinematics == nullptr)
        return;
    auto rotation = kinematics->FirstChildElement("rotation");
    if(rotation != nullptr && rotation->FirstChildElement() != nullptr)
    {
        std::unique_ptr<ValueProvider> provider = parseProvider(rotation->FirstChildElement(), entity, handler, nullptr, functions, cloneHandler);
        if(provider != nullptr)
            entity->bindRotationController(std::move(provider));
    }
    std::unique_ptr<ValueProvider> posX = findController(kinematics, entity, handler, nullptr, "position", "axis", "x", functions, cloneHandler);
    std::unique_ptr<ValueProvider> posY = findController(kinematics, entity, handler, nullptr, "position", "axis", "y", functions, cloneHandler);
    if(posX != nullptr || posY != nullptr)
        entity->bindPositionController(std::move(posX), std::move(posY));
}

std::unique_ptr<ValueProvider> LevelFileLoader::findController(
    const tinyxml2::XMLElement* xml,
    AnimatedGraphics* animated,
    VariableHandler* handler,
    Stoppable* stoppable,
    const std::string& childName,
    const std::string& propertyName,
    const std::string& propertyValue,
    std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
    CloneHandler& cloneHandler)
{
    for(auto iterator = xml->FirstChildElement(childName.c_str());
        iterator != nullptr; iterator = iterator->NextSiblingElement(childName.c_str()))
    {
        const char* value = iterator->Attribute(propertyName.c_str());
        if(value == nullptr || value != propertyValue)
            continue;
        if(iterator->FirstChildElement() == nullptr)
            throw std::runtime_error(utility::translateKey("SubTag"));
        return parseProvider(iterator->FirstChildElement(), animated, handler, stoppable, functions, cloneHandler);
    }
    return nullptr;
}

std::unique_ptr<ParticleTrail> LevelFileLoader::parseTrail(
    AnimatedGraphics* animated,
    const tinyxml2::XMLElement* xml,
    ResourceManager& resourceManager,
    std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
    CloneHandler& cloneHandler)
{
    if(auto xmltrail = xml->FirstChildElement("trailing"))
    {
        float distance = 100;
        xmltrail->QueryFloatAttribute("spawnDist", &distance);
        float minSpeed = xmltrail->FloatAttribute("speedMin");
        if(auto xmlani = xmltrail->FirstChildElement("animation"))
            if(auto animation = std::unique_ptr<Animation>(parseAnimation(xmlani, animated, animated, resourceManager, functions, cloneHandler)))
                return std::unique_ptr<ParticleTrail>(new ParticleTrail(std::move(animation), distance, minSpeed));
    }
    return nullptr;
}
