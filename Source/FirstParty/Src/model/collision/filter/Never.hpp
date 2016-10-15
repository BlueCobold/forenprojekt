#pragma once

#ifndef NEVER_HPP
#define NEVER_HPP

#include "CollisionFilter.hpp"

class Never : public CollisionFilter
{
    virtual bool onShouldCollide(Entity& entityA, Entity& entityB) override
    {
        return false;
    }
};

#endif // NEVER_HPP
