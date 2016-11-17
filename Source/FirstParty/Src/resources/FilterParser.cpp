
#include "FilterParser.hpp"

#include "FilterContext.hpp"
#include "ProviderParser.hpp"
#include "../model/EntitySpawn.hpp"
#include "../model/collision/filter/Always.hpp"
#include "../model/collision/filter/ApplyForceFilter.hpp"
#include "../model/collision/filter/ChangeBallSpawnFilter.hpp"
#include "../model/collision/filter/ChangeBallVelocityFilter.hpp"
#include "../model/collision/filter/ChangeGravityFilter.hpp"
#include "../model/collision/filter/Never.hpp"
#include "../model/collision/filter/PropertyFilter.hpp"
#include "../model/collision/filter/SpawnEntity.hpp"

void FilterParser::parse(const tinyxml2::XMLElement& xml)
{
    m_target.bindCollisionFilter(getFilter(xml));
}

std::unique_ptr<CollisionFilter> FilterParser::getFilter(const tinyxml2::XMLElement& xml) const
{
    std::string name(xml.Name());
    if(name == "always")
        return std::unique_ptr<CollisionFilter>(new Always());
    else if(name == "never")
        return std::unique_ptr<CollisionFilter>(new Never());
    else if(name == "spawnEntity")
    {
        auto name = xml.Attribute("name");
        if(!name)
            throw std::runtime_error(utility::translateKey("@SpawnWithoutName"));

        Entity* spawned(m_parser ? m_parser(name) : nullptr);
        if(spawned == nullptr)
            throw std::runtime_error(utility::replace(utility::translateKey("@UnknownEntityName"), name));

        std::unique_ptr<CollisionFilter> subFilter;
        if(auto subXml = xml.FirstChildElement())
            subFilter = getFilter(*subXml);

        auto spawnFunc = m_context.spawnFunc;
        std::unique_ptr<SpawnEntity> filter(new SpawnEntity([=](const Entity& owner, const Entity& spawned)
        {
            spawnFunc(owner.getPosition(), spawned);
        }, m_target, *spawned, std::move(subFilter)));
        return std::move(filter);
    }
    else if(name == "copySpawnLocation")
    {
        std::unique_ptr<CollisionFilter> subFilter;
        if(auto subXml = xml.FirstChildElement())
            subFilter = getFilter(*subXml);

        std::unique_ptr<ChangeBallSpawnFilter> filter(new ChangeBallSpawnFilter(std::move(subFilter)));
        return std::move(filter);
    }
    else if(name == "setBallVelocity")
    {
        std::unique_ptr<CollisionFilter> subFilter;
        if(auto subXml = xml.FirstChildElement())
            subFilter = getFilter(*subXml);

        std::unique_ptr<ChangeBallVelocityFilter> filter(new ChangeBallVelocityFilter(
            xml.FloatAttribute("x"), xml.FloatAttribute("y"), std::move(subFilter)));
        return std::move(filter);
    }
    else if(name == "applyForce")
    {
        std::unique_ptr<CollisionFilter> subFilter;
        if(auto subXml = xml.FirstChildElement())
            subFilter = getFilter(*subXml);

        b2Vec2 force(xml.FloatAttribute("x"), xml.FloatAttribute("y"));
        std::unique_ptr<ApplyForceFilter> filter(new ApplyForceFilter(force, std::move(subFilter)));
        return std::move(filter);

    }
    else if(name == "changeGravity")
    {
        bool target = true;//std::string("entity") == child->Attribute("target");
        b2Vec2 gravity(xml.FloatAttribute("x"), xml.FloatAttribute("y"));
        std::unique_ptr<ChangeGravityFilter> filter(new ChangeGravityFilter(m_context.gravity, gravity, target, &m_context.variableHandler));
        auto context = ProviderContext(filter.get(), nullptr, filter.get(), filter.get(), m_context.cloneHandler)
                                      .withFunctions(m_templates.functions);
        ProviderParser parser(context);
        std::unique_ptr<ValueProvider> provider(parser.parseSingle(*xml.FirstChildElement()));
        filter->bindProvider(std::move(provider));
        return std::move(filter);
    }
    else if(name == "propertyFilter")
    {
        bool target = std::string("entity") == xml.Attribute("target");
        std::unique_ptr<PropertyFilter> filter(new PropertyFilter(target, &m_context.variableHandler));
        auto context = ProviderContext(filter.get(), nullptr, filter.get(), filter.get(), m_context.cloneHandler)
                                      .withFunctions(m_templates.functions);
        ProviderParser parser(context);
        std::unique_ptr<ValueProvider> provider(parser.parseSingle(*xml.FirstChildElement()));
        filter->bindProvider(std::move(provider));
        return std::move(filter);
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("@UnknownFilter"), xml.Name()));
}
