#pragma once

#ifndef ENTITY_PARSER_HPP
#define ENTITY_PARSER_HPP

#include "AnimationContext.hpp"
#include "Config.hpp"
#include "ColliderContext.hpp"
#include "FilterContext.hpp"
#include "ProviderParser.hpp"
#include "TemplateParser.hpp"
#include "../animation/ParticleTrail.hpp"
#include "../model/Entity.hpp"
#include "../model/EntityFactory.hpp"
#include "../model/EntitySpawn.hpp"

#include <memory>
#include <vector>

#include <Box2D/Box2D.h>
#include <tinyxml2.h>

class ResourceManager;

struct EntitySet
{
    struct MetaInfo
    {
        int targetPoints;
        int numberOfTargets;
        int bonusTargetPoints;

        MetaInfo() : 
            targetPoints(0),
            numberOfTargets(0),
            bonusTargetPoints(0)
        { }
    };

    std::unique_ptr<Entity> entity;
    std::vector<EntitySpawn> spawns;
    MetaInfo meta;

    EntitySet()
    { }

    EntitySet(EntitySet&& other) :
        entity(std::move(other.entity)),
        spawns(std::move(other.spawns)),
        meta(other.meta)
    { }

    EntitySet& operator= (EntitySet&& other)
    {
        entity = std::move(other.entity);
        spawns = std::move(other.spawns);
        meta = other.meta;
        return *this;
    }
};

class EntityParser
{
public:
    EntityParser(const AnimationContext& context,
                 const FilterContext& filterContext,
                 const ColliderContext& colliderContext,
                 Config& config,
                 Templates& templates,
                 b2World& world,
                 EntityFactory::DeliveryCallback spawn) :
        m_context(context),
        m_filterContext(filterContext),
        m_colliderContext(colliderContext),
        m_config(config),
        m_templates(templates),
        m_world(world),
        m_spawn(spawn)
    { }

    EntitySet parse(const tinyxml2::XMLElement& xml) const;

    EntitySet parseFromTemplate(const std::string& name) const;

private:

    struct Override
    {
        std::string name;
        const tinyxml2::XMLElement* xml;

        Override() :
            xml(nullptr)
        { }
    };
    
    std::unique_ptr<ParticleTrail> parseTrail(const tinyxml2::XMLElement& xml) const;
    
    EntitySet parseFromOverride(const std::string& name) const;

    EntitySet parseSpawns(const tinyxml2::XMLElement& xml) const;

    void fillProperties(EntitySet& entities, const tinyxml2::XMLElement& xml) const;

    const tinyxml2::XMLElement* findPhysicsTag(const tinyxml2::XMLElement& xml) const;
    
    const tinyxml2::XMLElement* findShapeTag(const tinyxml2::XMLElement& xml) const;

    Override findOverrideSourceName(const std::string& repName) const;
    
    AnimationContext m_context;
    FilterContext m_filterContext;
    ColliderContext m_colliderContext;
    Templates& m_templates;
    Config& m_config;
    b2World& m_world;
    EntityFactory::DeliveryCallback m_spawn;
};

#endif // ENTITY_PARSER_HPP
