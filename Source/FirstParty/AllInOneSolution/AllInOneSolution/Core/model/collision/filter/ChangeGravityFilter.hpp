#pragma once

#ifndef CHANGE_GRAVITY_FILTER_HPP
#define CHANGE_GRAVITY_FILTER_HPP

#include <Box2D/Common/b2Math.h>

#include "PropertyFilter.hpp"

class ChangeGravityFilter : public PropertyFilter
{
public:
    ChangeGravityFilter(
            b2Vec2& targetGravity,
            const b2Vec2& gravity,
            const bool useEntityProperties,
            VariableHandler* handler = nullptr) :
        PropertyFilter(useEntityProperties, handler),
        m_gravity(gravity),
        m_targetGravity(targetGravity)
    { }

    virtual ~ChangeGravityFilter()
    { }

    virtual bool shouldCollide(Entity* entityA, Entity* entityB) override
    {
        bool should = PropertyFilter::shouldCollide(entityA, entityB);
        if(should)
            m_targetGravity = m_gravity;
        return should;
    }

private:

    b2Vec2 m_gravity;
    b2Vec2& m_targetGravity;
};

#endif // CHANGE_GRAVITY_FILTER_HPP
