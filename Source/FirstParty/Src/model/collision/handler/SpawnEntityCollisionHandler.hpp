#pragma once

#ifndef SPAWN_ENTITY_COLLISION_HANDLER_HPP
#define SPAWN_ENTITY_COLLISION_HANDLER_HPP

#include "CollisionHandler.hpp"

class SpawnEntityCollisionHandler : public CollisionHandler
{
public:
    typedef std::function<void(const Entity& owner, const Entity& spawned, const b2Vec2& contactPoint)> SpawnHandler;

    SpawnEntityCollisionHandler(const SpawnHandler& spawnHandler,
                                Entity& owner,
                                Entity& spawn) :
        m_handler(spawnHandler),
        m_owner(owner),
        m_spawn(spawn),
        m_spawned(false)
    { }

private:
    SpawnHandler m_handler;
    const Entity& m_owner;
    const Entity& m_spawn;
    bool m_spawned;

    void handleCollision(Entity& entityA, Entity& entityB, const b2Vec2& point, const float impulse) override
    {
        if(!m_spawned || m_spawn.isRespawnable())
        {
            m_spawned = true;
            m_handler(m_owner, m_spawn, point);
        }
    }
};

#endif // SPAWN_ENTITY_COLLISION_HANDLER_HPP
