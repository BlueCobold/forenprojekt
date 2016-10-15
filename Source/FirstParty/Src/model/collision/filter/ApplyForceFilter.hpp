#pragma once

#ifndef APPLY_FORCE_FILTER_HPP
#define APPLY_FORCE_FILTER_HPP

#include "ActionFilter.hpp"
#include "../../Ball.hpp"

class ApplyForceFilter : public ActionFilter
{
public:

    ApplyForceFilter(const b2Vec2& force, std::unique_ptr<CollisionFilter> child) :
        ActionFilter(std::move(child)),
        m_force(force)
    { }

    virtual ~ApplyForceFilter()
    { }

private:
    b2Vec2 m_force;

    virtual bool onShouldCollide(Entity& entityA, Entity& entityB) override
    {
        bool collides = ActionFilter::onShouldCollide(entityA, entityB);
        if(collides)
        {
            if(entityA.getType() == Entity::Ball)
                entityA.getBody()->ApplyForce(m_force, entityA.getBody()->GetWorldCenter(), false);
            else if(entityB.getType() == Entity::Ball)
                entityB.getBody()->ApplyForce(m_force, entityB.getBody()->GetWorldCenter(), false);
        }
        return collides;
    }
};

#endif // APPLY_FORCE_FILTER_HPP
