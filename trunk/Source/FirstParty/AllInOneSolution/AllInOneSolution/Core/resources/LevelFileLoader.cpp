#include "LevelFileLoader.hpp"

#include "../animation/provider/Absolute.hpp"
#include "../animation/provider/Adder.hpp"
#include "../animation/provider/AngleProvider.hpp"
#include "../animation/provider/CachedProvider.hpp"
#include "../animation/provider/Clamp.hpp"
#include "../animation/provider/FloatToInt.hpp"
#include "../animation/provider/Inverse.hpp"
#include "../animation/provider/Maximum.hpp"
#include "../animation/provider/Minimum.hpp"
#include "../animation/provider/Modulo.hpp"
#include "../animation/provider/Multiplier.hpp"
#include "../animation/provider/Negate.hpp"
#include "../animation/provider/Nop.hpp"
#include "../animation/provider/Pow.hpp"
#include "../animation/provider/RandomProvider.hpp"
#include "../animation/provider/SetVariable.hpp"
#include "../animation/provider/Sine.hpp"
#include "../animation/provider/StaticProvider.hpp"
#include "../animation/provider/Step.hpp"
#include "../animation/provider/Stop.hpp"
#include "../animation/provider/Substractor.hpp"
#include "../animation/provider/TimeProvider.hpp"
#include "../animation/provider/VariableProvider.hpp"

#include <map>
#include <vector>

sf::SoundBuffer* LevelFileLoader::parseSound(tinyxml2::XMLElement* xml,
    ResourceManager& resourceManager)
{
     sf::SoundBuffer* sound = resourceManager.getSoundBuffer(xml->Attribute("name"));

     return sound;
}

void LevelFileLoader::parseConstants(tinyxml2::XMLElement* xml,
    VariableHandler* holder)
{
    std::map<std::string, std::unique_ptr<ValueProvider>> map;
    for(auto var = xml->FirstAttribute(); var != nullptr; var = var->Next())
    {
        holder->setValueOf(var->Name(), var->FloatValue());
    }
}

std::unique_ptr<Animation> LevelFileLoader::parseAnimation(tinyxml2::XMLElement* xml,
    AnimatedGraphics* animated,
    VariableHandler* handler,
    std::unique_ptr<ValueProvider> provider,
    ResourceManager& resourceManager,
    std::unordered_map<std::string, tinyxml2::XMLElement*>* functions)
{
    tinyxml2::XMLElement* frameIndex = xml->FirstChildElement("frameindex");
    int frames = 1;
    if(frameIndex != nullptr)
        frames = frameIndex->IntAttribute("frames");
    
    sf::Texture* texture = resourceManager.getTexture(xml->Attribute("texture"));
    int width, height;
    if(xml->Attribute("width") != nullptr)
        width = xml->IntAttribute("width");
    else
        width = texture->getSize().x;
    if(xml->Attribute("height") != nullptr)
        height = xml->IntAttribute("height");
    else
        height = texture->getSize().y;

    bool rotate = xml->BoolAttribute("rotate");
    sf::Vector2f offset;
    offset.x = xml->FloatAttribute("x");
    offset.y = xml->FloatAttribute("y");
    sf::Vector2f origin;
    origin.x = xml->FloatAttribute("midx");
    origin.y = xml->FloatAttribute("midy");
    bool horizontal = true;
    if(xml->Attribute("alignment") != nullptr)
        horizontal = std::string("vertical") != xml->Attribute("alignment");
    std::unique_ptr<Animation> anim(new Animation(std::move(provider), frames, width, height, rotate, origin, offset, horizontal));
    sf::Vector2f sourceOffset;
    sourceOffset.x = xml->FloatAttribute("srcx");
    sourceOffset.y = xml->FloatAttribute("srcy");

    anim->bindTexture(*texture, sourceOffset);

    std::vector<std::unique_ptr<Animation>> subAnimations;

    std::unique_ptr<ValueProvider> xProvider = findController(xml, animated, handler, "position", "axis", "x", functions);
    std::unique_ptr<ValueProvider> yProvider = findController(xml, animated, handler, "position", "axis", "y", functions);
    anim->bindPositionController(std::move(xProvider), std::move(yProvider));

    std::unique_ptr<ValueProvider> xScaleProvider = findController(xml, animated, handler, "scale", "axis", "x", functions);
    std::unique_ptr<ValueProvider> yScaleProvider = findController(xml, animated, handler, "scale", "axis", "y", functions);
    anim->bindScaleController(std::move(xScaleProvider), std::move(yScaleProvider));

    std::unique_ptr<ValueProvider> red = findController(xml, animated, handler, "color", "channel", "red", functions);
    std::unique_ptr<ValueProvider> green = findController(xml, animated, handler, "color", "channel", "green", functions);
    std::unique_ptr<ValueProvider> blue = findController(xml, animated, handler, "color", "channel", "blue", functions);
    std::unique_ptr<ValueProvider> alpha = findController(xml, animated, handler, "color", "channel", "alpha", functions);
    anim->bindColorController(std::move(red), std::move(green), std::move(blue), std::move(alpha));

    if(auto blend = xml->Attribute("blending"))
    {
        sf::BlendMode mode = sf::BlendAlpha;
        if(std::string("add") == blend)
            mode = sf::BlendAdd;
        else if(std::string("mul") == blend)
            mode = sf::BlendMultiply;
        anim->setBlending(mode);
    }

    tinyxml2::XMLElement* rotation = xml->FirstChildElement("rotation");
    if(rotation != nullptr && rotation->FirstChildElement() != nullptr)
    {
        std::unique_ptr<ValueProvider> rotProvider = parseProvider(rotation->FirstChildElement(), animated, handler, functions);
        anim->bindRotationController(std::move(rotProvider));
    }

    tinyxml2::XMLElement* constants = xml->FirstChildElement("constants");
    if(constants != nullptr)
        parseConstants(constants, anim.get());

    return anim;
}

std::vector<std::unique_ptr<ValueProvider>> LevelFileLoader::parseProviders(tinyxml2::XMLElement* xml, 
    AnimatedObject* animated,
    VariableHandler* handler,
    std::unordered_map<std::string, tinyxml2::XMLElement*>* functions)
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
            return parseProviders(function->second, animated, handler, functions);
        }
        std::unique_ptr<ValueProvider> provider = parseProvider(child, animated, handler, functions);
        if(provider != nullptr)
            providers.push_back(std::move(provider));
    }
    return providers;
}

std::unique_ptr<ValueProvider> LevelFileLoader::parseProvider(tinyxml2::XMLElement* xml, 
    AnimatedObject* animated,
    VariableHandler* handler,
    std::unordered_map<std::string, tinyxml2::XMLElement*>* functions)
{
    if(std::string(xml->Name())=="time")
        return std::unique_ptr<TimeProvider>(new TimeProvider(animated));
    else if(std::string(xml->Name())=="stop")
        return std::unique_ptr<Stop>(new Stop(animated));
    else if(std::string(xml->Name())=="angle")
        return std::unique_ptr<AngleProvider>(new AngleProvider(animated));
    else if(std::string(xml->Name())=="static")
        return std::unique_ptr<StaticProvider>(new StaticProvider(xml->FloatAttribute("value")));
    else if(std::string(xml->Name())=="var")
        return std::unique_ptr<VariableProvider>(new VariableProvider(handler, xml->Attribute("name")));
    else if(std::string(xml->Name())=="setVar")
        return std::unique_ptr<SetVariable>(new SetVariable(handler, xml->Attribute("name"),
        std::move(parseProviders(xml, animated, handler, functions)[0])));
    else if(std::string(xml->Name())=="abs")
        return std::unique_ptr<Absolute>(new Absolute(std::move(parseProviders(xml, animated, handler, functions)[0])));
    else if(std::string(xml->Name())=="sine")
        return std::unique_ptr<Sine>(new Sine(std::move(parseProviders(xml, animated, handler, functions)[0])));
    else if(std::string(xml->Name())=="cache")
        return std::unique_ptr<CachedProvider>(new CachedProvider(std::move(parseProviders(xml, animated, handler, functions)[0])));
    else if(std::string(xml->Name())=="int")
        return std::unique_ptr<FloatToInt>(new FloatToInt(std::move(parseProviders(xml, animated, handler, functions)[0])));
    else if(std::string(xml->Name())=="add")
        return std::unique_ptr<Adder>(new Adder(std::move(parseProviders(xml, animated, handler, functions))));
    else if(std::string(xml->Name())=="mul")
        return std::unique_ptr<Multiplier>(new Multiplier(std::move(parseProviders(xml, animated, handler, functions))));
    else if(std::string(xml->Name())=="min")
        return std::unique_ptr<Minimum>(new Minimum(std::move(parseProviders(xml, animated, handler, functions))));
    else if(std::string(xml->Name())=="max")
        return std::unique_ptr<Maximum>(new Maximum(std::move(parseProviders(xml, animated, handler, functions))));
    else if(std::string(xml->Name())=="mod")
        return std::unique_ptr<Modulo>(new Modulo(std::move(parseProviders(xml, animated, handler, functions))));
    else if(std::string(xml->Name())=="pow")
        return std::unique_ptr<Pow>(new Pow(std::move(parseProviders(xml, animated, handler, functions))));
    else if(std::string(xml->Name())=="nop")
        return std::unique_ptr<Nop>(new Nop(std::move(parseProviders(xml, animated, handler, functions))));
    else if(std::string(xml->Name())=="clamp")
        return std::unique_ptr<Clamp>(new Clamp(std::move(parseProviders(xml, animated, handler, functions))));
    else if(std::string(xml->Name())=="step")
        return std::unique_ptr<Step>(new Step(std::move(parseProviders(xml, animated, handler, functions))));
    else if(std::string(xml->Name())=="sub")
        return std::unique_ptr<Substractor>(new Substractor(std::move(parseProviders(xml, animated, handler, functions))));
    else if(std::string(xml->Name())=="neg")
        return std::unique_ptr<Negate>(new Negate(std::move(parseProviders(xml, animated, handler, functions)[0])));
    else if(std::string(xml->Name())=="random")
        return std::unique_ptr<RandomProvider>(new RandomProvider(xml->FloatAttribute("min"), xml->FloatAttribute("max")));
    else if(std::string(xml->Name())=="inv")
        return std::unique_ptr<Inverse>(new Inverse(std::move(parseProviders(xml, animated, handler, functions)[0])));
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
        auto providers = parseProviders(function->second, animated, handler, functions);
        if(providers.size()>1)
            throw std::runtime_error(utility::replace(utility::translateKey("TemplateChild"), funcName));
        return std::move(providers[0]);
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("Unknown"), xml->Name()));
}

std::unique_ptr<Animation> LevelFileLoader::parseAnimation(tinyxml2::XMLElement* xml, 
    AnimatedGraphics* animated,
    VariableHandler* handler,
    ResourceManager& resourceManager,
    std::unordered_map<std::string, tinyxml2::XMLElement*>* functions)
{
    tinyxml2::XMLElement* frameIndex = xml->FirstChildElement("frameindex");
    std::unique_ptr<ValueProvider> provider;
    if(frameIndex == nullptr)
        provider.reset(new StaticProvider(0));
    else
    {
        tinyxml2::XMLElement* child = frameIndex->FirstChildElement();
        if(child == nullptr)
            throw std::runtime_error(utility::translateKey("NoProvider"));
        provider = std::move(parseProvider(child, animated, handler, functions));
    }

    return parseAnimation(xml, animated, handler, std::move(provider), resourceManager, functions);
}

std::unordered_map<std::string, tinyxml2::XMLElement*> LevelFileLoader::parseList(
     tinyxml2::XMLElement* xml, const std::string& name, const std::string& key)
{
    std::unordered_map<std::string, tinyxml2::XMLElement*> tempList;

    for(auto iterator = xml->FirstChildElement(name.c_str());
        iterator != nullptr; iterator = iterator->NextSiblingElement(name.c_str()))
    {
        tempList.insert(std::make_pair<std::string, tinyxml2::XMLElement*>(
            std::string(iterator->Attribute(key.c_str())), iterator));
    }

    return std::move(tempList);
}

std::vector<std::string> LevelFileLoader::parseGrid(tinyxml2::XMLElement* xml)
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

void LevelFileLoader::parseKinematics(tinyxml2::XMLElement* element,
    Entity* entity,
    VariableHandler* handler,
    std::unordered_map<std::string, tinyxml2::XMLElement*>* functions)
{
    tinyxml2::XMLElement* kinematics = element->FirstChildElement("kinematics");
    if(kinematics == nullptr)
        return;
    tinyxml2::XMLElement* rotation = kinematics->FirstChildElement("rotation");
    if(rotation != nullptr && rotation->FirstChildElement() != nullptr)
    {
        std::unique_ptr<ValueProvider> provider = parseProvider(rotation->FirstChildElement(), entity, handler, functions);
        if(provider != nullptr)
            entity->bindRotationController(std::move(provider));
    }
    std::unique_ptr<ValueProvider> posX = findController(kinematics, entity, handler, "position", "axis", "x", functions);
    std::unique_ptr<ValueProvider> posY = findController(kinematics, entity, handler, "position", "axis", "y", functions);
    if(posX != nullptr || posY != nullptr)
        entity->bindPositionController(std::move(posX), std::move(posY));
}

std::unique_ptr<ValueProvider> LevelFileLoader::findController(
    tinyxml2::XMLElement* xml,
    AnimatedGraphics* animated,
    VariableHandler* handler,
    const std::string& childName,
    const std::string& propertyName,
    const std::string& propertyValue,
    std::unordered_map<std::string, tinyxml2::XMLElement*>* functions)
{
    for(auto iterator = xml->FirstChildElement(childName.c_str());
        iterator != nullptr; iterator = iterator->NextSiblingElement(childName.c_str()))
    {
        if(std::string(iterator->Attribute(propertyName.c_str())) != propertyValue)
            continue;
        if(iterator->FirstChildElement() == nullptr)
            throw std::runtime_error(utility::translateKey("SubTag"));
        return parseProvider(iterator->FirstChildElement(), animated, handler, functions);
    }
    return nullptr;
}
