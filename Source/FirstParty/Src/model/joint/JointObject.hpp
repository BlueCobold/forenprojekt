#pragma once

#ifndef JOINTOBJECT_HPP
#define JOINTOBJECT_HPP

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/Joints/b2Joint.h>
#include <Box2D/Dynamics/b2World.h>

#include <vector>

/// basis for all other joint types
class JointObject
{
public:
    enum JointType
    {
        Unknown,
        SingleRevolute,
        SinglePrismatic,
        SingleDistance
    };

private:
    b2World* m_world;
    b2Joint* m_joint;
    JointType m_type;

public:
    JointObject(b2World* world, JointType type) :
        m_world(world),
        m_joint(nullptr),
        m_type(type)
    { }

    JointObject(JointType type) :
        m_world(nullptr),
        m_joint(nullptr),
        m_type(type)
    { }

    virtual ~JointObject()
    {
        m_joint = nullptr;
    }

    JointType getType() const
    {
        return m_type;
    }

    virtual void update() = 0;

    virtual bool reinstall(b2Body* body) = 0;

    virtual void copyFrom(const JointObject* other)
    {
        m_world = other->m_world;
    }

protected:
    virtual void bindBodies(b2Body* bodyA, b2Body* bodyB) = 0;

    void create(const b2JointDef& jointDef)
    {
        m_joint = getWorld()->CreateJoint(&jointDef);
    }

    b2Joint* getJoint()
    {
        return m_joint;
    }

    b2World* getWorld()
    {
        return m_world;
    }

    virtual std::vector<b2Vec2> getAnchorPoints() = 0;
};

#endif
