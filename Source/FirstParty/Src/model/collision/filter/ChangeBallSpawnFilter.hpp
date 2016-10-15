#pragma once

#ifndef CHANGE_BALL_SPAWN_FILTER_HPP
#define CHANGE_BALL_SPAWN_FILTER_HPP

#include "ActionFilter.hpp"
#include "../../Ball.hpp"

class ChangeBallSpawnFilter : public ActionFilter
{
public:
    ChangeBallSpawnFilter(std::unique_ptr<CollisionFilter> child) :
        ActionFilter(std::move(child))
    { }

    virtual ~ChangeBallSpawnFilter()
    { }

private:
    virtual bool onShouldCollide(Entity& entityA, Entity& entityB) override
    {
        bool collides = ActionFilter::onShouldCollide(entityA, entityB);
        if(collides)
            (dynamic_cast<Ball&>(entityB)).checkpointReached(entityA.getPosition());
        return collides;
    }
};

#endif // CHANGE_BALL_SPAWN_FILTER_HPP
