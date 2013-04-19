#pragma once

#ifndef CHANGE_BALL_SPAWN_FILTER_HPP
#define CHANGE_BALL_SPAWN_FILTER_HPP

#include "ActionFilter.hpp"
#include "../../Ball.hpp"

#include <exception>

class ChangeBallSpawnFilter : public ActionFilter
{
public:

    ChangeBallSpawnFilter(std::unique_ptr<CollisionFilter> child) :
        ActionFilter(std::move(child))
    { }

    virtual ~ChangeBallSpawnFilter()
    { }

    virtual bool shouldCollide(Entity* entityA, Entity* entityB)
    {
        bool collides = ActionFilter::shouldCollide(entityA, entityB);
        if(collides)
            (dynamic_cast<Ball*>(entityB))->checkpointReached(entityA->getPosition());
        return collides;
    }
};

#endif // CHANGE_BALL_SPAWN_FILTER_HPP
