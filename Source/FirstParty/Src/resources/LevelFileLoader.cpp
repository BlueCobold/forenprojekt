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
        std::unique_ptr<ValueProvider> provider = m_providerParser.parseSingle(*rotation->FirstChildElement());
        if(provider != nullptr)
            entity->bindRotationController(std::move(provider));
    }

    ProviderParserContext context(handler, entity, entity, entity, cloneHandler);
    ControllerParser parser(context);
    auto posX = parser.findController(*kinematics, "position", "axis", "x");
    auto posY = parser.findController(*kinematics, "position", "axis", "y");
    if(posX != nullptr || posY != nullptr)
        entity->bindPositionController(std::move(posX), std::move(posY));
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
