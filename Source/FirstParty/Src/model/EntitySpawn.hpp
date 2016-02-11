#pragma once

#ifndef ENTITY_SPAWN_HPP
#define ENTITY_SPAWN_HPP

#include "Entity.hpp"

#include <memory>

struct EntitySpawn
{
public:
    EntitySpawn(std::unique_ptr<Entity> target) :
        target(std::move(target)),
        respawnAt(-1.f)
    { }

    EntitySpawn(std::unique_ptr<Entity> target, const float time) :
        target(std::move(target)),
        respawnAt(time)
    { }

    EntitySpawn(EntitySpawn&& source) :
        target(std::move(source.target)),
        respawnAt(source.respawnAt)
    { }

    EntitySpawn& operator=(EntitySpawn&& source)
    {
        target = std::move(source.target);
        respawnAt = source.respawnAt;
        return *this;
    }

    std::unique_ptr<Entity> target;
    float respawnAt;
};

#endif // ENTITY_SPAWN_HPP
