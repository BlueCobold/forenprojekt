#pragma once

#ifndef APPLY_FORCE_FILTER_HPP
#define APPLY_FORCE_FILTER_HPP

#include "ActionFilter.hpp"
#include "../../Ball.hpp"

class ApplyForceFilter : public ActionFilter
{
public:

    ApplyForceFilter(
        const b2Vec2& force,
        std::unique_ptr<CollisionFilter> child) :
            m_force(force),
            ActionFilter(std::move(child))
    { }

    virtual ~ApplyForceFilter()
    { }

    virtual bool shouldCollide(Entity* entityA, Entity* entityB) override
    {
        bool collides = ActionFilter::shouldCollide(entityA, entityB);
        if(collides)
        {
            if(entityA && entityA->getType() == Entity::Ball)
                entityA->getBody()->ApplyForce(m_force, entityA->getBody()->GetWorldCenter(), false);
            else if(entityB && entityB->getType() == Entity::Ball)
                entityB->getBody()->ApplyForce(m_force, entityB->getBody()->GetWorldCenter(), false);
        }
        return collides;
    }

private:

    b2Vec2 m_force;
};

#endif // APPLY_FORCE_FILTER_HPP
