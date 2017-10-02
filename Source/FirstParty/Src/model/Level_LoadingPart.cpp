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
#include "../resources/ResourceManager.hpp"
#include "../resources/SpriteSheet.hpp"
#include "../resources/ValueParser.hpp"
#include "../Utility.hpp"
#include "../MacHelper.hpp"

#include <tinyxml2.h>

#include <algorithm>
#include <memory>

LevelInfo Level::load(bool loadInfoOnly)
{
    if(number() != 0)
    {
        if(m_number == 0) // Level start from 1
            throw std::runtime_error(utility::replace(utility::translateKey("@InvalidLevelNumber"), filename()));

        if(!isOriginal())
            throw std::runtime_error(utility::replace(utility::translateKey("@NoOriginalFile"), filename()));
    }

    tinyxml2::XMLDocument doc;

    std::string filePath;
    if(number() == 0 && !m_filename.empty())
        filePath = m_filename;
    else
        filePath = resourcePath() + filename();
    doc.LoadFile(filePath.c_str());

    if(doc.Error())
    {
        doc.PrintError();
        throw std::runtime_error(utility::replace(utility::translateKey("@InvalidXml"), filePath));
    }

    auto levelXml = doc.FirstChildElement("level");
    if(!levelXml)
        throw std::runtime_error(utility::replace(utility::translateKey("@InvalidXml"), filePath));

    LevelInfo info;
    if(auto levelinfo = levelXml->FirstChildElement("levelinfo"))
    {
        if(auto name = levelinfo->Attribute("name"))
            info.name = name;

        parsePreview(*levelinfo, info);

        // get optional Attribute
        if(auto gameplay = levelinfo->FirstChildElement("gameplay"))
            parseGameplayAttributes(*gameplay, info);
        else
        {
            m_remainingBall = -1;
            m_remainingTime = -1.f;
            m_totalTime = -1.f;
            m_initialTime = -1.f;
        }
    }
    if(loadInfoOnly)
        return info;

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

    if(auto medal = levelXml->FirstChildElement("medal"))
    {
        // get Medal values
        medal->QueryIntAttribute("bronze", &m_bronzeMedal);
        medal->QueryIntAttribute("silver", &m_silverMedal);
        medal->QueryIntAttribute("gold", &m_goldMedal);
    }

    // ==Parse grid==
    auto grid = levelXml->FirstChildElement("grid");
    if(!grid)
        throw std::runtime_error(utility::replace(utility::translateKey("@InvalidXml"), filePath));

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
        BackgroundParser backgroundParser(context, *m_resourceManager);
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
    return info;
}

std::unique_ptr<Entity> Level::parseEntity(
    const tinyxml2::XMLElement* entityXml,
    const std::string& templateName,
    Templates& templates)
{

    ProviderContext context(this, nullptr, nullptr, nullptr, m_cloneHandler);
    AnimationContext animationContext(context, *m_resourceManager, m_defaultTargetBuffer);
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

void Level::parsePreview(const tinyxml2::XMLElement& levelinfo, LevelInfo& infoToFill)
{
    if(auto info = levelinfo.FirstChildElement("infoimage"))
    {
        SpriteSheet::SpriteData sprite;
        auto spriteName = info->Attribute("sprite");
        auto sheetName = spriteName ? info->Attribute("spriteSheet") : nullptr;
        auto sheet = sheetName ? m_resourceManager->getSpriteSheet(sheetName) : nullptr;
        if(sheet != nullptr)
            sprite = sheet->get(spriteName);
        if(auto srcx = info->Attribute("srcx"))
            sprite.x = utility::stringTo<int>(srcx);
        if(auto srcy = info->Attribute("srcy"))
            sprite.y = utility::stringTo<int>(srcy);
        if(auto width = info->Attribute("width"))
            sprite.width = utility::stringTo<int>(width);
        if(auto height = info->Attribute("height"))
            sprite.height = utility::stringTo<int>(height);
        auto tex = m_resourceManager->getTexture(sheet ? sheet->getTextureName() : "GuiElements");
        sf::Sprite baseSprite(*tex,
                              sf::IntRect(sprite.x, sprite.y,
                                          sprite.width, sprite.height));
        baseSprite.setOrigin(sprite.originX, sprite.originY);
        infoToFill.preview = Sprite(baseSprite, sprite.blendMode);
    }
}

void Level::parseGameplayAttributes(const tinyxml2::XMLElement& gameplay, LevelInfo& infoToFill)
{
    gameplay.QueryIntAttribute("maxBalls", &infoToFill.maxBalls);

    gameplay.QueryFloatAttribute("time", &infoToFill.time);

    if(infoToFill.maxBalls > 0)
        m_remainingBall = infoToFill.maxBalls;
    else
        m_remainingBall = -1;

    if(infoToFill.time > 0)
    {
        m_remainingTime = infoToFill.time;
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
