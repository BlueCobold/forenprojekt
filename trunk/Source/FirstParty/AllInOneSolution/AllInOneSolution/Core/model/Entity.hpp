#pragma once

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "AnimatedGraphics.hpp"
#include "PhysicalObject.hpp"
#include "SoundObject.hpp"
#include "collision/CollisionHandler.hpp"

#include <memory> // std::unique_ptr
#include <string>

/// This class will be used to draw objects that have a binding
/// to Box2D 
class Entity : public PhysicalObject, public AnimatedGraphics, public SoundObject
{
public:

    enum Type {Teeter, Ball, Target, None};

private:

    std::string m_name;
    Type m_type;
    float m_lastTime;
    bool m_killed;
    bool m_collideWithBall;
    Animation* m_updatingAni;
    std::unique_ptr<CollisionHandler> m_collisionHandler;
    std::map<std::string, float> m_variables;

public:
    Entity(Type type);
    virtual ~Entity();

    virtual void update(const float value);
    virtual void restartAt(const float value);

    void setName(std::string name);
    const Type& getType() const;

    void kill();
    bool killed() const;

    void setCollideWithBall(bool value);
    bool isCollideWithBall() const;

    virtual float getValueOf(const std::string& name) const;
    virtual void setValueOf(const std::string& name, const float value);

    void bindCollisionHandler(std::unique_ptr<CollisionHandler> handler);
    void onCollide(Entity* partner);
};

#endif // ENTITY_HPP
