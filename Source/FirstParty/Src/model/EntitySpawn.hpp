#pragma once

#ifndef ENTITY_SPAWN_HPP
#define ENTITY_SPAWN_HPP

#include "Entity.hpp"

#include <memory>

struct EntitySpawn
{
public:
    EntitySpawn(std::unique_ptr<Entity> target) :
        m_target(std::move(target)),
        m_respawnAt(-1.f)
    { }

    EntitySpawn(std::unique_ptr<Entity> target, const double time) :
        m_target(std::move(target)),
        m_respawnAt(time)
    { }

    EntitySpawn(EntitySpawn&& source) :
        m_target(std::move(source.m_target)),
        m_respawnAt(source.m_respawnAt)
    { }

    EntitySpawn& operator=(EntitySpawn&& source)
    {
        m_target = std::move(source.m_target);
        m_respawnAt = source.m_respawnAt;
        return *this;
    }

    Entity* getTarget() const
    {
        return m_target.get();
    }

    std::unique_ptr<Entity> releaseTarget()
    {
        return std::move(std::move(m_target));
    }

    double getRespawnTime() const
    {
        return m_respawnAt;
    }

private:
    std::unique_ptr<Entity> m_target;
    double m_respawnAt;
};

#endif // ENTITY_SPAWN_HPP
