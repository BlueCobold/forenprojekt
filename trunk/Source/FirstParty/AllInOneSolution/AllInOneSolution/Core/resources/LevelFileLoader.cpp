#include "LevelFileLoader.hpp"

#include "../animation/AngleCalculator.hpp"
#include "../animation/AngleProvider.hpp"
#include "../animation/TimeCalculator.hpp"
#include "../animation/TimeProvider.hpp"
#include "../animation/StaticProvider.hpp"

#include <vector>

std::unique_ptr<ValueCalculator> LevelFileLoader::parseStaticCalculator(tinyxml2::XMLElement* xml)
{
    std::unique_ptr<StaticProvider> provider(new StaticProvider(xml->FloatAttribute("value")));
    std::unique_ptr<ValueCalculator> calc(new ValueCalculator(std::move(provider)));
    return calc;
}

std::unique_ptr<ValueCalculator> LevelFileLoader::parseTimedCalculator(tinyxml2::XMLElement* xml, const AnimatedGraphics* entity, const int frames)
{
    std::unique_ptr<TimeProvider> provider(new TimeProvider(entity));
    std::unique_ptr<ValueCalculator> calc(new TimeCalculator(std::move(provider),
        xml->FloatAttribute("delay"), frames));
    return calc;
}

std::unique_ptr<ValueCalculator> LevelFileLoader::parseAngleCalculator(tinyxml2::XMLElement* xml, const Entity* entity, const int frames)
{
    std::unique_ptr<AngleProvider> provider(new AngleProvider(entity));
    std::unique_ptr<ValueCalculator> calc(new AngleCalculator(std::move(provider),
        xml->FloatAttribute("min"), xml->FloatAttribute("step"), frames));
    return calc;
}

std::unique_ptr<Animation> LevelFileLoader::parseAnimation(tinyxml2::XMLElement* xml,
    const AnimatedGraphics* animated,
    std::unique_ptr<ValueCalculator> calculator,
    ResourceManager& resourceManager)
{
    int width = xml->IntAttribute("width");
    int height = xml->IntAttribute("height");
    bool rotate = xml->BoolAttribute("rotate");
    std::unique_ptr<Animation> anim(new Animation(std::move(calculator), width, height, rotate));
    sf::Vector2f offset;
    sf::Texture* texture = resourceManager.getTexture(xml->Attribute("texture"));
    offset.x = xml->FloatAttribute("x");
    offset.y = xml->FloatAttribute("y");
    anim->bindTexture(*texture, offset);

    std::vector<std::unique_ptr<Animation>> subAnimations;

    for(auto subs = xml->FirstChildElement("animation"); subs != nullptr; subs = subs->NextSiblingElement("animation"))
    {
        std::unique_ptr<ValueCalculator> calculator = parseCalculator(subs, animated);
        subAnimations.push_back(std::move(parseAnimation(subs, animated, std::move(calculator), resourceManager)));
    }
    if(subAnimations.size() > 0)
        anim->bindSubAnimations(subAnimations);

    return anim;
}

std::unique_ptr<ValueCalculator> LevelFileLoader::parseCalculator(tinyxml2::XMLElement* xml, 
    const AnimatedGraphics* animated)
{
    int frames = xml->IntAttribute("frames");
    tinyxml2::XMLElement* timed = xml->FirstChildElement("timed");
    tinyxml2::XMLElement* stat = xml->FirstChildElement("static");
    std::unique_ptr<ValueCalculator> calculator;
    if(timed != nullptr)
        return parseTimedCalculator(timed, animated, frames);
    else if(stat != nullptr)
        return parseStaticCalculator(stat);
    return nullptr;
}

std::unique_ptr<Animation> LevelFileLoader::parseAnimation(tinyxml2::XMLElement* xml, 
    const AnimatedGraphics* animated,
    ResourceManager& resourceManager)
{
    std::unique_ptr<ValueCalculator> calculator = std::move(parseCalculator(xml, animated));
    if(calculator == nullptr)
        return nullptr;

    return parseAnimation(xml, animated, std::move(calculator), resourceManager);
}

std::unique_ptr<Animation> LevelFileLoader::parseAnimation(tinyxml2::XMLElement* xml,
    const Entity* entity,
    ResourceManager& resourceManager)
{
    int frames = xml->IntAttribute("frames");

    std::unique_ptr<ValueCalculator> calculator = parseCalculator(xml, entity);
    if(calculator == nullptr)
    {
        tinyxml2::XMLElement* angled = xml->FirstChildElement("angled");
        if(angled != nullptr)
            calculator = parseAngleCalculator(angled, entity, frames);
        else
            return nullptr;
    }
    
    return parseAnimation(xml, entity, std::move(calculator), resourceManager);
}
