#pragma once

#ifndef SPAWN_ENTITY_HPP
#define SPAWN_ENTITY_HPP

#include "ActionFilter.hpp"

#include <functional>
#include <exception>
#include <memory>

class SpawnEntity : public ActionFilter
{
public:
    typedef std::function<void(const Entity* owner, const Entity* spawned)> SpawnHandler;

    SpawnEntity(
            const SpawnHandler& spawnHandler,
            const Entity* owner,
            const Entity* spawn,
            std::unique_ptr<CollisionFilter> child) :
        ActionFilter(std::move(child)),
        m_owner(owner),
        m_spawn(spawn),
        m_handler(spawnHandler),
        m_spawned(false)
    {
        if(m_spawn == nullptr || m_owner == nullptr)
            throw std::runtime_error("SpawnEntity created without owner or child!");
    }

    virtual ~SpawnEntity()
    { }

    virtual bool shouldCollide(Entity* entityA, Entity* entityB) override
    {
        bool collides = ActionFilter::shouldCollide(entityA, entityB);
        if((!m_spawned || m_spawn->isRespawnable()) && collides)
        {
            m_spawned = true;
            m_handler(m_owner, m_spawn);
        }
        return collides;
    }

private:

    bool m_spawned;
    SpawnHandler m_handler;
    const Entity* m_owner;
    const Entity* m_spawn;
};

#endif // SPAWN_ENTITY_HPP
