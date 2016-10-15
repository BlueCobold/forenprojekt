#pragma once

#ifndef ALWAYS_HPP
#define ALWAYS_HPP

#include "CollisionFilter.hpp"

class Always : public CollisionFilter
{
    virtual bool onShouldCollide(Entity& entityA, Entity& entityB) override
    {
        return true;
    }
};

#endif // ALWAYS_HPP
