#include "LevelFileLoader.hpp"

std::unique_ptr<ValueCalculator> LevelFileLoader::parseTimedCalculator(tinyxml2::XMLElement* xml, const Entity* entity, const int frames)
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

std::unique_ptr<Animation> LevelFileLoader::parseAnimation(tinyxml2::XMLElement* xml, const Entity* entity)
{
    std::unique_ptr<ValueCalculator> calculator;

    int frames = xml->IntAttribute("frames");
    int width = xml->IntAttribute("width");
    int height = xml->IntAttribute("height");
    tinyxml2::XMLElement* timed = xml->FirstChildElement("timed");
    tinyxml2::XMLElement* angled = xml->FirstChildElement("angled");
    if(timed != nullptr)
        calculator = parseTimedCalculator(timed, entity, frames);
    else if(angled != nullptr)
        calculator = parseAngleCalculator(angled, entity, frames);
    else
        return nullptr;

    std::unique_ptr<Animation> anim(new Animation(std::move(calculator), width, height));

    return anim;
}