#pragma once

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "AnimatedGraphics.hpp"
#include "PhysicalObject.hpp"

#include <memory> // std::unique_ptr
#include <string>

/// This class will be used to draw objects that have a binding
/// to Box2D 
class Entity : public PhysicalObject, public AnimatedGraphics
{
private:

    std::string m_name;

public:

    Entity();
    virtual ~Entity();

    virtual void update(const float value);

    void setName(std::string name)
    {
        m_name = name;
    }
};

#endif // ENTITY_HPP
