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
    typedef std::function<void(const Entity& owner, const Entity& spawned)> SpawnHandler;

    SpawnEntity(const SpawnHandler& spawnHandler,
                Entity& owner,
                Entity& spawn,
                std::unique_ptr<CollisionFilter> child) :
        ActionFilter(std::move(child)),
        m_handler(spawnHandler),
        m_owner(owner),
        m_spawn(spawn),
        m_spawned(false)
    { }

    virtual ~SpawnEntity()
    { }

private:
    SpawnHandler m_handler;
    const Entity& m_owner;
    const Entity& m_spawn;
    bool m_spawned;

    virtual bool onShouldCollide(Entity& entityA, Entity& entityB) override
    {
        bool collides = ActionFilter::onShouldCollide(entityA, entityB);
        if((!m_spawned || m_spawn.isRespawnable()) && collides)
        {
            m_spawned = true;
            m_handler(m_owner, m_spawn);
        }
        return collides;
    }
};

#endif // SPAWN_ENTITY_HPP
