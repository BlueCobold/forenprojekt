#include "Level.hpp"

#include "../rendering/Shader.hpp"
#include "../resources/AnimationParser.hpp"
#include "../resources/AppConfig.hpp"
#include "../resources/BackgroundParser.hpp"
#include "../resources/ControllerParser.hpp"
#include "../resources/EntityParser.hpp"
#include "../resources/JointParser.hpp"
#include "../resources/LevelFileLoader.hpp"
#include "../resources/PathHelper.hpp"
#include "../resources/PhysicsParser.hpp"
#include "../resources/ValueParser.hpp"
#include "../Utility.hpp"
#include "../MacHelper.hpp"

#include <tinyxml2.h>

#include <algorithm>

void Level::load()
{
#ifndef LEVELTESTING
    if(m_number == 0) // Level start from 1
        throw std::runtime_error(utility::replace(utility::translateKey("InvalidLevelNumber"), filename()));

    if(!isOriginal())
        throw std::runtime_error(utility::replace(utility::translateKey("NoOriginalFile"), filename()));

#endif
    tinyxml2::XMLDocument doc;

    std::string filePath;
#ifdef LEVELTESTING
    if(number() == 0)
        filePath = m_filename;
    else
#endif
    filePath = resourcePath() + filename();
    doc.LoadFile(filePath.c_str());

    if(doc.Error())
    {
        doc.PrintError();
        throw std::runtime_error(utility::replace(utility::translateKey("InvalidXml"), filePath));
    }

    auto levelXml = doc.FirstChildElement("level");
    if(!levelXml)
        throw std::runtime_error(utility::replace(utility::translateKey("InvalidXml"), filePath));
    
    if(auto setup = levelXml->FirstChildElement("setup"))
        setup->QueryUnsignedAttribute("defaultBufferId", &m_defaultTargetBuffer);
    if(!Shader::isUsable())
    {
        if(auto setup = levelXml->FirstChildElement("noShaderSetup"))
            setup->QueryUnsignedAttribute("defaultBufferId", &m_defaultTargetBuffer);
    }

    auto constants = levelXml->FirstChildElement("constants");
    if(constants != nullptr)
        ValueParser::parseConstants(*constants, *this);

    // get optional Attribute
    if(auto gameplay = levelXml->FirstChildElement("gameplay"))
        parseGameplayAttributes(gameplay);
    else
    {
        m_remainingBall = -1;
        m_remainingTime = -1.f;
        m_totalTime = -1.f;
        m_initialTime = -1.f;
    }

    // get Medal values
    m_bronzeMedal = levelXml->FirstChildElement("medal")->IntAttribute("bronze");
    m_silverMedal = levelXml->FirstChildElement("medal")->IntAttribute("silver");
    m_goldMedal = levelXml->FirstChildElement("medal")->IntAttribute("gold");

    // get LevelName
    m_levelName = levelXml->FirstChildElement("levelinfo")->Attribute("name");

    // ==Parse grid==
    auto grid = levelXml->FirstChildElement("grid");
    if(!grid)
        throw std::runtime_error(utility::replace(utility::translateKey("InvalidXml"), filePath));

    // Get tile size
    unsigned int size = static_cast<unsigned int>(grid->IntAttribute("size"));
    m_height = grid->FloatAttribute("height");
    m_width = grid->FloatAttribute("width");

    Templates templates;
    TemplateParser templateParser(pathname());
    if(auto xmlTemplates = levelXml->FirstChildElement("templates"))
        templates = templateParser.parse(*xmlTemplates);

    if(auto objects = levelXml->FirstChildElement("objects"))
        templateParser.parseObjects(*objects, templates);
    
    for(auto it = begin(templates.objects.entities); it != end(templates.objects.entities); ++it)
    {
        if(auto entity = parseEntity(*it, "", templates))
            m_entities.push_back(std::move(entity));
    }
    if(templates.objects.backgrounds.size() > 0)
    {
        auto context = ProviderContext(nullptr, nullptr, nullptr, nullptr, m_cloneHandler).withFunctions(templates.functions);
        BackgroundParser backgroundParser(context, m_resourceManager);
        auto size = sf::Vector2u(static_cast<unsigned int>(m_width), static_cast<unsigned int>(m_height));
        m_background = backgroundParser.parse(*templates.objects.backgrounds[0], size, m_defaultTargetBuffer);
    }

    // Separate the lines for easier processing
    std::vector<std::string> lines = LevelFileLoader::parseGrid(*grid);

    for(size_t row = 0; row < lines.size(); ++row)
        for(std::size_t column = 0; column < lines[row].length(); column += 2)
        {
            std::string name = lines[row].substr(column, 2);

            // Ignore empty 'tiles'
            if(name == "  ")
                continue;

            auto pos = sf::Vector2u(static_cast<unsigned int>((column / 2) * size),
                                    static_cast<unsigned int>(row * size));
            if(auto entity = parseEntity(nullptr, name, templates))
            {
                auto gridPos = b2Vec2(static_cast<float>(utility::toMeter(pos.x)),
                                      static_cast<float>(utility::toMeter(pos.y)));
                entity->setPosition(gridPos + entity->getPosition());
                m_entities.push_back(std::move(entity));
            }
        }

    tinyxml2::XMLElement* world = levelXml->FirstChildElement("world");
    // Load world properties
    tinyxml2::XMLElement* gravity = world->FirstChildElement("gravity");
    m_defaultGravity = b2Vec2(gravity->FloatAttribute("x"), gravity->FloatAttribute("y"));
    m_gravity = m_defaultGravity;
    m_world.SetContactListener(&m_contactListener);

    // setup scrollview
    m_scrollView.setLevelSize(sf::Vector2f(getWidth(), getHeight()));

    // get the fucking ball
    auto ballIt = end(m_entities);

    for(auto it = begin(m_entities); it != end(m_entities); ++it)
    {
        if((*it)->getType() == Entity::Ball && ballIt == end(m_entities))
        {
            ballIt = it;
            m_ball = dynamic_cast<Ball*>((*it).get());
            m_ball->setFieldDimension(b2Vec2(m_width,m_height));
        }
    }

    if(m_ball == nullptr)
        throw std::runtime_error("No ball located in the level!");

    // sort entity by her drawOrder
    std::sort(m_entities.begin(), m_entities.end(), compareDrawOrder);

    m_ball->registerForCheckpointChanges([this](){
        createLabelAt(*m_ball, "green", "checkpoint");
    });

    m_remainingTarget = m_totalTarget;
}

std::unique_ptr<Entity> Level::parseEntity(
    const tinyxml2::XMLElement* entityXml,
    const std::string& templateName,
    Templates& templates)
{

    ProviderContext context(this, nullptr, nullptr, nullptr, m_cloneHandler);
    AnimationContext animationContext(context, m_resourceManager, m_defaultTargetBuffer);
    FilterContext filterContext(m_gravity, [this](const b2Vec2& position, const Entity& spawn)
    {
        prepareEntityForSpawn(position, spawn);
    }, *this, m_cloneHandler);
    
    ColliderContext colliderContext([this](const b2Vec2& position, const Entity& spawn)
    {
        prepareEntityForSpawn(position, spawn);
    }, [this](int time)
    {
        m_remainingTime += static_cast<float>(time);
    }, [this]()->Ball&
    {
        return *m_ball;
    }, [this](const sf::Vector2f& position, const std::string& fontName, const std::string& text)
    {
        createLabelAt(position, fontName, text);
    }, [this]()->bool
    {
        return m_invulnerableGoody.isActive();
    }, *this, *this, m_cloneHandler);

    EntityParser parser(animationContext, filterContext, colliderContext, m_config.getConfig(), templates, m_world,
                        [this](std::unique_ptr<Entity>& created)
    {
        m_entitiesToSpawn.push_back(std::move(created));
    });

    auto entities = entityXml != nullptr ? parser.parse(*entityXml) : parser.parseFromTemplate(templateName);

    if(entities.meta.targetPoints != 0)
        m_normalTargetPoints = entities.meta.targetPoints;

    if(entities.meta.bonusTargetPoints != 0)
        m_bonusTargetPoints = entities.meta.bonusTargetPoints;

    m_totalTarget += entities.meta.numberOfTargets;
    
    std::move(begin(entities.spawns), end(entities.spawns), std::back_inserter(m_unspawnedEntities));
    return std::move(entities.entity);
}

void Level::parseGameplayAttributes(const tinyxml2::XMLElement* xml)
{
    int balls = xml->IntAttribute("maxBalls");
    float remainingTime = xml->FloatAttribute("time");
    if(balls > 0)
        m_remainingBall = balls;
    else
        m_remainingBall = -1;

    if(remainingTime > 0)
    {
        m_remainingTime = remainingTime;
        m_totalTime = m_remainingTime;
        m_initialTime = m_totalTime;
    }
    else
    {
        m_remainingTime = -1.f;
        m_totalTime = -1.f;
        m_initialTime = -1.f;
    }
}
