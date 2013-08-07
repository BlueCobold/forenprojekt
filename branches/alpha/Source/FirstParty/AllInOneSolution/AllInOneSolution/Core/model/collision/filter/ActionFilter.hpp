#pragma once

#ifndef ACTION_FILTER_HPP
#define ACTION_FILTER_HPP

#include "CollisionFilter.hpp"

#include <exception>
#include <memory>

class ActionFilter : public CollisionFilter
{
public:
    ActionFilter(std::unique_ptr<CollisionFilter> child) : m_child(std::move(child))
    {
        if(m_child == nullptr)
            throw std::runtime_error("Action-filters need a child-filter!");
    }

    virtual ~ActionFilter()
    { }

    virtual bool shouldCollide(Entity* entityA, Entity* entityB) override
    {
        return m_child->shouldCollide(entityA, entityB);
    }

private:

    std::unique_ptr<CollisionFilter> m_child;
};

#endif // ACTION_FILTER_HPP
