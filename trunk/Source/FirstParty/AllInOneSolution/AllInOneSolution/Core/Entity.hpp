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
public:

    enum Type {Teeter, Ball, Target, None};

private:

    std::string m_name;
    Type m_type;
    float m_lastTime;
    bool m_killed;

public:

    Entity(Type type);
    virtual ~Entity();

    virtual void update(const float value);

    void setName(std::string name);

    const Type& getType() const;

    virtual void Entity::restartAt(const float value);

    void kill();

    bool killed() const;
};

#endif // ENTITY_HPP
