
#include "EntityParser.hpp"

#include "AnimationParser.hpp"
#include "ColliderParser.hpp"
#include "ControllerParser.hpp"
#include "FilterParser.hpp"
#include "JointParser.hpp"
#include "LevelFileLoader.hpp"
#include "PhysicsParser.hpp"
#include "ValueParser.hpp"
#include "../model/Ball.hpp"
#include "../model/EntityFactory.hpp"
#include "../model/Teeter.hpp"

EntitySet EntityParser::parse(const tinyxml2::XMLElement& xml) const
{
    auto& cloneHandler = m_context.providerContext.cloneHandler;

    bool respawnable = false;
    xml.QueryBoolAttribute("respawnable", &respawnable);

    bool autoStop = false;
    xml.QueryBoolAttribute("stopWithLastAnimation", &autoStop);
    
    EntitySet entities;
    std::unique_ptr<Entity> entity;
    if(auto name = xml.Attribute("base"))
    {
        entities = parseFromTemplate(name);
        if(entities.entity == nullptr)
            return entities;
        entity = std::move(entities.entity);
    }
    else
    {
        std::string typeName;
        if(auto type = xml.Attribute("type"))
            typeName = type;

        if(typeName == "teeter")
            entity = std::unique_ptr<Teeter>(new Teeter(m_config.get<float>("MouseScale"), cloneHandler));
        else if(typeName == "ball")
        {
            Entity* spawn = nullptr;
            if(auto onRespawnXml = xml.FirstChildElement("onRespawn"))
            {
                auto onRespawn = parseSpawns(*onRespawnXml);
                spawn = onRespawn.entity.get();
                entities.spawns.push_back(EntitySpawn(std::move(onRespawn.entity)));
                std::move(begin(onRespawn.spawns), end(onRespawn.spawns), std::back_inserter(onRespawn.spawns));
            }

            float autoKillSpeed = xml.FloatAttribute("autoKillSpeed");
            auto ball = std::unique_ptr<Ball>(new Ball(m_config.get<float>("BallResetTime"), autoKillSpeed, cloneHandler, spawn));

            auto context = ProviderContext(m_context.providerContext.variableHandler, ball.get(), ball.get(), ball.get(), cloneHandler)
                                          .withFunctions(m_templates.functions);
            LevelFileLoader loader(context, m_context.resourceManager, m_context.defaultTargetBuffer);
            ball->bindTrail(loader.parseTrail(xml));
            entity = std::move(ball);
        }
        else if(typeName == "target")
        {
            entity = std::unique_ptr<Entity>(new Entity(Entity::Target, cloneHandler, respawnable, autoStop));
            entities.meta.targetPoints = xml.IntAttribute("points");
            if(entities.meta.targetPoints == 0)
                entities.meta.targetPoints = 100;
            entities.meta.numberOfTargets++;
        }
        else if(typeName == "bonustarget")
        {
            entity = std::unique_ptr<Entity>(new Entity(Entity::BonusTarget, cloneHandler, respawnable, autoStop));
            entities.meta.bonusTargetPoints = xml.IntAttribute("points");
            if(entities.meta.bonusTargetPoints == 0)
                entities.meta.bonusTargetPoints = 10;
        }
        else if(typeName == "entityFactory")
        {
            float min = 0, max = 0;
            xml.QueryFloatAttribute("minDelayTime", &min);
            xml.QueryFloatAttribute("maxDelayTime", &max);

            std::string productName = xml.Attribute("productName");

            b2Vec2 spawnOffset;
            xml.QueryFloatAttribute("spawnOffsetX", &spawnOffset.x);
            xml.QueryFloatAttribute("spawnOffsetY", &spawnOffset.y);

            EntitySet product = parseFromTemplate(productName);
            std::move(begin(product.spawns), end(product.spawns), std::back_inserter(entities.spawns));

            auto factory = std::unique_ptr<EntityFactory>(new EntityFactory(cloneHandler, respawnable, autoStop, 
                                                                            std::move(product.entity), min, max, spawnOffset));

            factory->setOwnSpeedTransfer(xml.BoolAttribute("transferOwnSpeed"));
            factory->registerForDelivery(m_spawn);
            entity = std::move(factory);
        }
        else // No type or unknown type specified => normal Entity
        {
            entity = std::unique_ptr<Entity>(new Entity(Entity::None, cloneHandler, respawnable, autoStop));
        }
    }
    
    entities.entity = std::move(entity);
    fillProperties(entities, xml);
    return entities;
}

void EntityParser::fillProperties(EntitySet& entities, const tinyxml2::XMLElement& xml) const
{
    auto& cloneHandler = m_context.providerContext.cloneHandler;

    sf::Vector2u position, offset;
    xml.QueryUnsignedAttribute("x", &position.x);
    xml.QueryUnsignedAttribute("y", &position.y);
    xml.QueryUnsignedAttribute("offsetx", &offset.x);
    xml.QueryUnsignedAttribute("offsety", &offset.y);
    position += offset;
    auto physicalPosition = b2Vec2(static_cast<float>(utility::toMeter(position.x)),
                                   static_cast<float>(utility::toMeter(position.y)));
    
    auto entity = entities.entity.get();
    if(entity == nullptr)
        return;

    physicalPosition = physicalPosition + entity->getPosition();

    if(auto onKillXml = xml.FirstChildElement("onKill"))
    {
        auto onKill = parseSpawns(*onKillXml);
        entity->bindKillAnimationEntity(onKill.entity.get());
        entities.spawns.push_back(EntitySpawn(std::move(onKill.entity)));
        std::move(begin(onKill.spawns), end(onKill.spawns), std::back_inserter(onKill.spawns));
    }

    if(auto name = xml.Attribute("name"))
        entity->setName(name);
    if(auto name = xml.Attribute("newName"))
        entity->setName(name);
    
    auto z = entity->getDrawOrder();
    xml.QueryFloatAttribute("z", &z);
    xml.QueryFloatAttribute("draworder", &z);
    entity->setDrawOrder(z);

    auto context = ProviderContext(m_context.providerContext.variableHandler, entity, entity, entity, cloneHandler)
                                  .withFunctions(m_templates.functions);

    if(auto animationsTag = xml.FirstChildElement("animations"))
    {
        AnimationContext aniContext(context, m_context.resourceManager, m_context.defaultTargetBuffer);
        AnimationParser loader(aniContext);
        auto animations = loader.parseMultiple(*animationsTag);
        for(auto ani = begin(animations); ani != end(animations); ++ani)
            entity->bindAnimation(std::move(*ani));
    }

    if(auto constants = xml.FirstChildElement("constants"))
        ValueParser::parseConstants(*constants, *entity);

    // Load sound
    if(auto soundXml = xml.FirstChildElement("sound"))
    {
        auto sound = std::unique_ptr<SoundObject>(new SoundObject(std::string(soundXml->Attribute("name")), m_context.resourceManager.getSoundManager()));
        if(soundXml->Attribute("volume"))
            sound->fixVolume(soundXml->FloatAttribute("volume"));
        entity->bindCollisionSound(std::move(sound));
    }

    if(auto soundXml = xml.FirstChildElement("sounds"))
    {
        std::vector<std::unique_ptr<SoundTrigger>> otherSounds;
        for(auto sound = soundXml->FirstChildElement("sound"); sound != nullptr; sound = sound->NextSiblingElement())
        {
            std::string soundName = sound->Attribute("name");
            auto soundContext = ProviderContext(nullptr, nullptr, nullptr, nullptr, cloneHandler)
                                               .withFunctions(m_templates.functions);
            ProviderParser parser(soundContext);
            std::unique_ptr<ValueProvider> provider(parser.parseSingle(*sound->FirstChildElement()));
            otherSounds.push_back(std::unique_ptr<SoundTrigger>(new SoundTrigger(soundName, m_context.resourceManager.getSoundManager(), std::move(provider))));
        }
        entity->bindOtherSounds(std::move(otherSounds));
    }
    
    auto physic = findPhysicsTag(xml);
    auto shape = findShapeTag(xml);
    if(physic != nullptr && shape != nullptr)
    {
        PhysicsParser parser(ProviderContext(entity, entity, entity, entity, cloneHandler));
        auto physics = parser.parse(*physic, *shape);
        physics.bodyDef.bullet = entity->getType() == Entity::Ball;
        physics.bodyDef.position = physicalPosition;
        entity->bindDefs(physics.fixtureDef, physics.shapes, physics.bodyDef, &m_world);
        entity->bindRotationController(std::move(physics.controllers.rotation));
        entity->bindPositionController(std::move(*physics.controllers.position[0]), std::move(*physics.controllers.position[1]));
    }
    if(entity->hasPhysics())
        entities.entity->bindBody();
    entity->setPosition(physicalPosition);

    if(entity->hasPhysics())
    {
        if(auto collider = xml.FirstChildElement("onCollision"))
        {
            auto self = this;
            ColliderParser parser(m_colliderContext, *entity, m_templates, [&](const std::string& templateName)->Entity*
            {
                auto spawned = self->parseFromTemplate(templateName);
                std::move(begin(spawned.spawns), end(spawned.spawns), std::back_inserter(entities.spawns));
                auto result = spawned.entity.get();
                entities.spawns.push_back(std::move(spawned.entity));
                return result;
            });
            entity->bindCollisionHandler(parser.parse(*collider));
        }

        if(xml.Attribute("collideWithBall") != nullptr)
            entity->setCollideWithBall(xml.BoolAttribute("collideWithBall"));
        else
            entity->setCollideWithBall(true);
        
        if(xml.Attribute("collideWithOtherEntity") != nullptr)
            entity->setCollideWithOtherEntity(xml.BoolAttribute("collideWithOtherEntity"));
        else
            entity->setCollideWithOtherEntity(true);

        if(auto filter = xml.FirstChildElement("collides"))
        {
            if(auto child = filter->FirstChildElement())
            {
                auto self = this;
                auto f = [&](const std::string& templateName)->Entity*
                {
                    auto spawned = self->parseFromTemplate(templateName);
                    std::move(begin(spawned.spawns), end(spawned.spawns), std::back_inserter(entities.spawns));
                    auto result = spawned.entity.get();
                    entities.spawns.push_back(std::move(spawned.entity));
                    return result;
                };
                FilterParser parser(m_filterContext, *entity, m_templates, f);
                parser.parse(*child);
            }
        }
    }

    if(auto jointXml = xml.FirstChildElement("joints"))
    {
        ProviderContext context(entity, entity, entity, entity, cloneHandler);
        AnimationContext aniContext(context,
                                    m_context.resourceManager,
                                    m_context.defaultTargetBuffer);
        JointParser parser(aniContext,
                           m_world,
                           *entity->getBody());
        auto joints = parser.parse(*jointXml);
        for(auto it = begin(joints); it != end(joints); ++it)
        {
            for(auto ani = begin(it->animations); ani != end(it->animations); ++ani)
                entity->bindAnimation(std::move(*ani));
            entity->addJoint(std::move(it->joint));
        }
    }
}

// Takes a result and tries to find the origin by checking if it matches the source-name using a target-conversion-pattern
std::string buildOriginal(const std::string& targetPattern, const std::string& sourceName, const std::string& result)
{
    if(targetPattern == result)
        return sourceName;
    if(targetPattern.length() != 2 || sourceName.length() != 2 || result.length() != 2 || (targetPattern[0] != '*' && targetPattern[1] != '*'))
        return result;
    else if(targetPattern[0] == '*' && (targetPattern[1] == result[1] || targetPattern[1] == '*'))
        return std::string(1, result[0]) + sourceName[1];
    else if(targetPattern[1] == '*' && (targetPattern[0] == result[0] || targetPattern[0] == '*'))
        return std::string(1, sourceName[0]) + result[1];
    return "";
}

EntitySet EntityParser::parseFromTemplate(const std::string& name) const
{
    auto match = m_templates.entities.find(name);
    if(match != end(m_templates.entities))
        return parse(*match->second);

    return parseFromOverride(name);
}

EntityParser::Override EntityParser::findOverrideSourceName(const std::string& repName) const
{
    for(auto it = begin(m_templates.overrides); it != end(m_templates.overrides); ++it)
    {
        auto& potentialSource = *it->second;
        auto productRepName = it->first;
        std::string originalName;
        if(auto sourceRep = potentialSource.Attribute("rep"))
            originalName = buildOriginal(productRepName, sourceRep, repName);

        if(originalName.empty() || originalName == repName)
        {
            if(productRepName == repName)
                if(auto sourceName = potentialSource.Attribute("name"))
                    originalName = sourceName;
        }
        if(originalName.empty() || originalName == repName)
            continue;

        Override o;
        o.name = originalName;
        o.xml = &potentialSource;
        return o;
    }
    return Override();
}

EntitySet EntityParser::parseFromOverride(const std::string& name) const
{
    auto overrides = findOverrideSourceName(name);
    if(overrides.name.empty() || overrides.name == name || overrides.xml == nullptr)
        return EntitySet();

    auto& xml = *overrides.xml;
    // recurse deeper if this is an override only
    auto entities = parseFromTemplate(overrides.name);

    auto entity = entities.entity.get();
    if(entity == nullptr)
        return entities;

    if(auto aniXml = xml.FirstChildElement("animations"))
    {
        entity->applyOverrides([&](Animation* animation)
        {
            auto& providerContext = m_context.providerContext;
            auto context = ProviderContext(providerContext.variableHandler, entity, entity, entity, providerContext.cloneHandler)
                                            .withFunctions(m_templates.functions);

            ControllerParser controllerParser(context);
            auto position = controllerParser.parsePosition(*aniXml);
            animation->bindPositionController(std::move(*position[0]), std::move(*position[1]));

            auto scale = controllerParser.parseScale(*aniXml);
            animation->bindScaleController(std::move(*scale[0]), std::move(*scale[1]));

            auto color = controllerParser.parseColor(*aniXml);
            animation->bindColorController(std::move(*color[0]), std::move(*color[1]), std::move(*color[2]), std::move(*color[3]));

            animation->bindRotationController(controllerParser.parseRotation(*aniXml));

            if(auto setXml = aniXml->FirstChildElement("set"))
            {
                if(setXml->Attribute("rotate"))
                    animation->applyRotation(setXml->BoolAttribute("rotate"));
            }
            if(auto constants = aniXml->FirstChildElement("constants"))
                ValueParser::parseConstants(*constants, *animation);
        });
    }
    
    fillProperties(entities, xml);

    return entities;
}

EntitySet EntityParser::parseSpawns(const tinyxml2::XMLElement& xml) const
{
    auto action = xml.FirstChildElement("spawnEntity");
    if(action == nullptr)
        return EntitySet();

    auto name = action->Attribute("name");
    if(!name)
        throw std::runtime_error(utility::translateKey("SpawnWithoutName"));

    EntitySet entities = parseFromTemplate(name);
    if(entities.entity == nullptr)
        throw std::runtime_error(utility::replace(utility::translateKey("UnknownEntityName"), name));

    return entities;
}

const tinyxml2::XMLElement* EntityParser::findPhysicsTag(const tinyxml2::XMLElement& xml) const
{
    if(auto physics = xml.FirstChildElement("physics"))
    {
        if(auto physicsName = physics->Attribute("name"))
        {
            auto physicsTemplate = m_templates.physics.find(physicsName);
            if(physicsTemplate != end(m_templates.physics))
                return physicsTemplate->second;

            throw std::runtime_error(utility::replace(utility::translateKey("UnknownPhysicReference"), physicsName));
        }
        else
            return physics;
    }
    return nullptr;
}

const tinyxml2::XMLElement* EntityParser::findShapeTag(const tinyxml2::XMLElement& xml) const
{
    if(auto physics = xml.FirstChildElement("physics"))
    {
        if(auto shapeName = physics->Attribute("shape"))
        {
            auto shapeTemplate = m_templates.shapes.find(shapeName);
            if(shapeTemplate != end(m_templates.shapes))
                return shapeTemplate->second;
        }
        else
            return physics->FirstChildElement("shape");
    }
    return nullptr;
}
