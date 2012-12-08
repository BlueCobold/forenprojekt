#pragma once

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "AnimatedGraphics.hpp"
#include "PhysicalObject.hpp"

#include <memory> // std::unique_ptr
#include <string>

enum EntityType {ET_Teeter, ET_Ball, ET_Target, ET_None};

/// This class will be used to draw objects that have a binding
/// to Box2D 
class Entity : public PhysicalObject, public AnimatedGraphics
{
private:

    std::string m_name;
    EntityType m_type;
    float m_lastTime;
    bool m_killed;

public:

    Entity();
    virtual ~Entity();

    virtual void update(const float value);

    void bindType(EntityType type);

    void setName(std::string name);

    EntityType& getType();

    virtual void Entity::restartAt(const float value);

    void kill();

    bool killed();

};

#endif // ENTITY_HPP
