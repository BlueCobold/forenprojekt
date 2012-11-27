#include "LevelFileLoader.hpp"

#include "../animation/AngleCalculator.hpp"
#include "../animation/AngleProvider.hpp"
#include "../animation/TimeCalculator.hpp"
#include "../animation/TimeProvider.hpp"
#include "../animation/StaticProvider.hpp"

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

    return anim;
}

std::unique_ptr<Animation> LevelFileLoader::parseAnimation(tinyxml2::XMLElement* xml, 
    const AnimatedGraphics* animated,
    ResourceManager& resourceManager)
{
    int frames = xml->IntAttribute("frames");
    tinyxml2::XMLElement* timed = xml->FirstChildElement("timed");
    tinyxml2::XMLElement* stat = xml->FirstChildElement("static");
    std::unique_ptr<ValueCalculator> calculator;
    if(timed != nullptr)
        calculator = parseTimedCalculator(timed, animated, frames);
    else if(stat != nullptr)
        calculator = parseStaticCalculator(stat);
    else
        return nullptr;

    return parseAnimation(xml, animated, std::move(calculator), resourceManager);
}

std::unique_ptr<Animation> LevelFileLoader::parseAnimation(tinyxml2::XMLElement* xml,
    const Entity* entity,
    ResourceManager& resourceManager)
{
    int frames = xml->IntAttribute("frames");
    std::unique_ptr<ValueCalculator> calculator;
    tinyxml2::XMLElement* timed = xml->FirstChildElement("timed");
    tinyxml2::XMLElement* angled = xml->FirstChildElement("angled");
    tinyxml2::XMLElement* stat = xml->FirstChildElement("static");
    if(timed != nullptr)
        calculator = parseTimedCalculator(timed, entity, frames);
    else if(angled != nullptr)
        calculator = parseAngleCalculator(angled, entity, frames);
    else if(stat != nullptr)
        calculator = parseStaticCalculator(stat);
    else
        return nullptr;
    
    return parseAnimation(xml, entity, std::move(calculator), resourceManager);
}
