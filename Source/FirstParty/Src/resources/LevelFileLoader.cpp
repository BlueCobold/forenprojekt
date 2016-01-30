#include "LevelFileLoader.hpp"

#include "AnimationParser.hpp"
#include "ControllerParser.hpp"
#include "SpriteSheet.hpp"
#include "../animation/provider/ValueProvider.hpp"
#include "../rendering/Shader.hpp"

#include <map>
#include <vector>

std::vector<std::string> LevelFileLoader::parseGrid(const tinyxml2::XMLElement& xml)
{
    std::string data = xml.GetText();

    // Remove first and last line breaks
    data = data.substr(data.find_first_of('\n')+1, data.find_last_of('\n'));

    std::vector<std::string> lines;

    // Get the lines separated without the newline char
    for(std::size_t i = 0; i < data.length(); i = data.find('\n', i) + 1)
        lines.push_back(data.substr(i, data.find('\n', i) - i));

    return std::move(lines);
}

std::unique_ptr<ParticleTrail> LevelFileLoader::parseTrail(const tinyxml2::XMLElement& xml)
{
    if(auto xmltrail = xml.FirstChildElement("trailing"))
    {
        auto distance = 100.f;
        xmltrail->QueryFloatAttribute("spawnDist", &distance);
        auto minSpeed = xmltrail->FloatAttribute("speedMin");
        if(auto xmlani = xmltrail->FirstChildElement("animation"))
        {
            AnimationParser parser(m_context, m_resourceManager);
            if(auto animation = std::unique_ptr<Animation>(parser.parseSingle(*xmlani)))
                return std::unique_ptr<ParticleTrail>(new ParticleTrail(std::move(animation), distance, minSpeed));
        }
    }
    return nullptr;
}
