#include "LevelFileLoader.hpp"

#include "../animation/provider/Absolute.hpp"
#include "../animation/provider/Adder.hpp"
#include "../animation/provider/AngleProvider.hpp"
#include "../animation/provider/FloatToInt.hpp"
#include "../animation/provider/Inverse.hpp"
#include "../animation/provider/Maximum.hpp"
#include "../animation/provider/Minimum.hpp"
#include "../animation/provider/Modulo.hpp"
#include "../animation/provider/Multiplier.hpp"
#include "../animation/provider/Sine.hpp"
#include "../animation/provider/StaticProvider.hpp"
#include "../animation/provider/SetVariable.hpp"
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
    const AnimatedObject* animated,
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
    std::unique_ptr<ValueProvider> provider,
    ResourceManager& resourceManager)
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
    if(xml->Attribute("width") != nullptr)
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
    std::unique_ptr<Animation> anim(new Animation(std::move(provider), frames, width, height, rotate, origin, offset));
    sf::Vector2f sourceOffset;
    sourceOffset.x = xml->FloatAttribute("srcx");
    sourceOffset.y = xml->FloatAttribute("srcy");

    anim->bindTexture(*texture, sourceOffset);

    std::vector<std::unique_ptr<Animation>> subAnimations;

    std::unique_ptr<ValueProvider> xProvider = findPositionController(xml, animated, std::string("x"));
    std::unique_ptr<ValueProvider> yProvider = findPositionController(xml, animated, std::string("y"));
    if((xProvider != nullptr) || (yProvider != nullptr))
        anim->bindPositionController(std::move(xProvider), std::move(yProvider));

    tinyxml2::XMLElement* rotation = xml->FirstChildElement("rotation");
    if(rotation != nullptr && rotation->FirstChildElement() != nullptr)
    {
        std::unique_ptr<ValueProvider> rotProvider = parseProvider(rotation->FirstChildElement(), animated);
        if(rotProvider != nullptr)
            anim->bindRotationController(std::move(rotProvider));
    }

    tinyxml2::XMLElement* constants = xml->FirstChildElement("constants");
    if(constants != nullptr)
        parseConstants(constants, animated, anim.get());

    return anim;
}

std::vector<std::unique_ptr<ValueProvider>> LevelFileLoader::parseProviders(tinyxml2::XMLElement* xml, 
    AnimatedObject* animated)
{
    std::vector<std::unique_ptr<ValueProvider>> providers;
    for(auto child = xml->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        std::unique_ptr<ValueProvider> provider = parseProvider(child, animated);
        if(provider != nullptr)
            providers.push_back(std::move(provider));
    }
    return providers;
}

std::unique_ptr<ValueProvider> LevelFileLoader::parseProvider(tinyxml2::XMLElement* xml, 
    AnimatedObject* animated)
{
    if(std::string(xml->Name())=="time")
        return std::unique_ptr<TimeProvider>(new TimeProvider(animated));
    else if(std::string(xml->Name())=="angle")
        return std::unique_ptr<AngleProvider>(new AngleProvider(animated));
    else if(std::string(xml->Name())=="static")
        return std::unique_ptr<StaticProvider>(new StaticProvider(xml->FloatAttribute("value")));
    else if(std::string(xml->Name())=="var")
        return std::unique_ptr<VariableProvider>(new VariableProvider(animated, xml->Attribute("name")));
    else if(std::string(xml->Name())=="setVar")
        return std::unique_ptr<SetVariable>(new SetVariable(animated, xml->Attribute("name"),
        std::move(parseProviders(xml, animated)[0])));
    else if(std::string(xml->Name())=="abs")
        return std::unique_ptr<Absolute>(new Absolute(std::move(parseProviders(xml, animated)[0])));
    else if(std::string(xml->Name())=="sine")
        return std::unique_ptr<Sine>(new Sine(std::move(parseProviders(xml, animated)[0])));
    else if(std::string(xml->Name())=="int")
        return std::unique_ptr<FloatToInt>(new FloatToInt(std::move(parseProviders(xml, animated)[0])));
    else if(std::string(xml->Name())=="add")
        return std::unique_ptr<Adder>(new Adder(std::move(parseProviders(xml, animated))));
    else if(std::string(xml->Name())=="mul")
        return std::unique_ptr<Multiplier>(new Multiplier(std::move(parseProviders(xml, animated))));
    else if(std::string(xml->Name())=="min")
        return std::unique_ptr<Minimum>(new Minimum(std::move(parseProviders(xml, animated))));
    else if(std::string(xml->Name())=="max")
        return std::unique_ptr<Maximum>(new Maximum(std::move(parseProviders(xml, animated))));
    else if(std::string(xml->Name())=="mod")
        return std::unique_ptr<Modulo>(new Modulo(std::move(parseProviders(xml, animated))));
    else if(std::string(xml->Name())=="inv")
        return std::unique_ptr<Inverse>(new Inverse(std::move(parseProviders(xml, animated)[0])));
    else
        throw std::exception((std::string("Unknown value-provider specified: ") + xml->Name()).c_str());
}

std::unique_ptr<Animation> LevelFileLoader::parseAnimation(tinyxml2::XMLElement* xml, 
    AnimatedGraphics* animated,
    ResourceManager& resourceManager)
{
    tinyxml2::XMLElement* frameIndex = xml->FirstChildElement("frameindex");
    std::unique_ptr<ValueProvider> provider;
    if(frameIndex == nullptr)
        provider.reset(new StaticProvider(0));
    else
    {
        tinyxml2::XMLElement* child = frameIndex->FirstChildElement();
        if(child == nullptr)
            throw std::exception("<frameindex> specified, but no provider included");
        provider = std::move(parseProvider(child, animated));
    }

    return parseAnimation(xml, animated, std::move(provider), resourceManager);
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

void LevelFileLoader::parseKinematics(tinyxml2::XMLElement* element, Entity* entity)
{
    tinyxml2::XMLElement* kinematics = element->FirstChildElement("kinematics");
    if(kinematics == nullptr)
        return;
    tinyxml2::XMLElement* rotation = kinematics->FirstChildElement("rotation");
    if(rotation != nullptr && rotation->FirstChildElement() != nullptr)
    {
        std::unique_ptr<ValueProvider> provider = parseProvider(rotation->FirstChildElement(), entity);
        if(provider != nullptr)
            entity->bindRotationController(std::move(provider));
    }
    std::unique_ptr<ValueProvider> posX = findPositionController(kinematics, entity, "x");
    std::unique_ptr<ValueProvider> posY = findPositionController(kinematics, entity, "y");
    if(posX != nullptr || posY != nullptr)
        entity->bindPositionController(std::move(posX), std::move(posY));
}

std::unique_ptr<ValueProvider> LevelFileLoader::findPositionController(
    tinyxml2::XMLElement* xml,
    AnimatedGraphics* animated,
    const std::string& axis)
{
    for(auto iterator = xml->FirstChildElement("position");
        iterator != nullptr; iterator = iterator->NextSiblingElement("position"))
    {
        if(std::string(iterator->Attribute("axis")) != axis)
            continue;
        if(iterator->FirstChildElement() == nullptr)
            throw std::exception("The position element needs a mathematical sub-tag.");
        return parseProvider(iterator->FirstChildElement(), animated);
    }
    return nullptr;
}
