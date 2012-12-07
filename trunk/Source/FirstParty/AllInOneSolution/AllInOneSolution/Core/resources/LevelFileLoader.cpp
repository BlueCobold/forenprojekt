#include "LevelFileLoader.hpp"

#include "../animation/AngleProvider.hpp"
#include "../animation/TimeProvider.hpp"
#include "../animation/StaticProvider.hpp"

#include <vector>

std::unique_ptr<Animation> LevelFileLoader::parseAnimation(tinyxml2::XMLElement* xml,
    const AnimatedGraphics* animated,
    std::unique_ptr<ValueProvider> provider,
    ResourceManager& resourceManager)
{
    tinyxml2::XMLElement* frameIndex = xml->FirstChildElement("frameindex");
    if(frameIndex == nullptr)
        return nullptr;
    int frames = frameIndex->IntAttribute("frames");
    int width = xml->IntAttribute("width");
    int height = xml->IntAttribute("height");
    bool rotate = xml->BoolAttribute("rotate");
    std::unique_ptr<Animation> anim(new Animation(std::move(provider), frames, width, height, rotate));
    sf::Vector2f offset;
    sf::Vector2f sourceOffset;
    sf::Texture* texture = resourceManager.getTexture(xml->Attribute("texture"));
    offset.x = xml->FloatAttribute("x");
    offset.y = xml->FloatAttribute("y");
    sourceOffset.x = xml->FloatAttribute("srcx");
    sourceOffset.y = xml->FloatAttribute("srcy");

    anim->bindTexture(*texture, offset, sourceOffset);

    std::vector<std::unique_ptr<Animation>> subAnimations;

    for(auto subs = xml->FirstChildElement("animation"); subs != nullptr; subs = subs->NextSiblingElement("animation"))
    {
        std::unique_ptr<ValueProvider> subProvider = parseProvider(subs, animated);
        subAnimations.push_back(std::move(parseAnimation(subs, animated, std::move(subProvider), resourceManager)));
    }
    if(subAnimations.size() > 0)
        anim->bindSubAnimations(subAnimations);

    return anim;
}

std::unique_ptr<ValueProvider> LevelFileLoader::parseProvider(tinyxml2::XMLElement* xml, 
    const AnimatedGraphics* animated)
{
    tinyxml2::XMLElement* timed = xml->FirstChildElement("time");
    tinyxml2::XMLElement* stat = xml->FirstChildElement("static");
    tinyxml2::XMLElement* angled = xml->FirstChildElement("angle");
    if(timed != nullptr)
        return std::unique_ptr<TimeProvider>(new TimeProvider(animated));
    else if(angled != nullptr)
        return std::unique_ptr<AngleProvider>(new AngleProvider(animated));
    else if(stat != nullptr)
        return std::unique_ptr<StaticProvider>(new StaticProvider(stat->FloatAttribute("value")));
    return nullptr;
}

std::unique_ptr<Animation> LevelFileLoader::parseAnimation(tinyxml2::XMLElement* xml, 
    const AnimatedGraphics* animated,
    ResourceManager& resourceManager)
{
    tinyxml2::XMLElement* frameIndex = xml->FirstChildElement("frameindex");
    if(frameIndex == nullptr)
        return nullptr;
    std::unique_ptr<ValueProvider> provider = std::move(parseProvider(frameIndex, animated));
    if(provider == nullptr)
        return nullptr;

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
