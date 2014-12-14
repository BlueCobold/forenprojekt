#pragma once

#ifndef JOINTOBJECT_HPP
#define JOINTOBJECT_HPP

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/Joints/b2Joint.h>
#include <Box2D/Dynamics/b2World.h>

/// basis for all other joint types
class JointObject
{
public:
    enum JointType {Unknown,
                    SingleRevolute,
                    SinglePrismatic,
                    SingleDistance};

private:
    b2World* m_world;
    b2Joint* m_joint;

    JointType m_type;

public:
    JointObject(b2World* world, JointType type) :
        m_joint(nullptr),
        m_world(world),
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

protected:
    virtual void bindBodys(b2Body* bodyA, b2Body* bodyB) = 0;

    b2Joint*& getJoint()
    {
        return m_joint;
    }

    b2World* getWorld()
    {
        return m_world;
    }
};

#endif