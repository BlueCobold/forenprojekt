#pragma once

#ifndef CHANGE_GRAVITY_FILTER_HPP
#define CHANGE_GRAVITY_FILTER_HPP

#include <Box2D/Common/b2Math.h>

#include "PropertyFilter.hpp"

class ChangeGravityFilter : public PropertyFilter
{
public:
    ChangeGravityFilter(
            b2World& world,
            const b2Vec2& gravity,
            const bool useEntityProperties,
            VariableHandler* handler = nullptr) :
        PropertyFilter(useEntityProperties, handler),
        m_world(world),
        m_gravity(gravity)
    { }

    virtual ~ChangeGravityFilter()
    { }

    virtual bool shouldCollide(Entity* entityA, Entity* entityB)
    {
        bool should = PropertyFilter::shouldCollide(entityA, entityB);
        if(should)
        {
            m_world.SetGravity(m_gravity);
        }
        return should;
    }

private:

    b2World& m_world;
    b2Vec2 m_gravity;
};

#endif // CHANGE_GRAVITY_FILTER_HPP
