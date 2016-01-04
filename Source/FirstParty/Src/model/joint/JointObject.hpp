#pragma once

#ifndef JOINTOBJECT_HPP
#define JOINTOBJECT_HPP

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/Joints/b2Joint.h>
#include <Box2D/Dynamics/b2World.h>

#include <functional>
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
    std::vector<std::function<void(const JointObject& src, JointObject& clone)>> m_beforeCopyCallbacks;
    std::vector<std::function<void(const JointObject& src, JointObject& clone)>> m_afterCopyCallbacks;

public:
    JointObject(b2World& world, JointType type) :
        m_world(&world),
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

    virtual bool reinstall(b2Body& body) = 0;

    void copyFrom(const JointObject& other)
    {
        for(auto it = begin(m_beforeCopyCallbacks); it != end(m_beforeCopyCallbacks); ++it)
            (*it)(other, *this);

        m_world = other.m_world;
        doCopyFrom(other);

        for(auto it = begin(m_beforeCopyCallbacks); it != end(m_beforeCopyCallbacks); ++it)
            (*it)(other, *this);
    }

    virtual std::vector<b2Vec2> getAnchorPoints() const = 0;

    virtual std::vector<b2Vec2> getAnchorOffsets() const = 0;

    void registerCopyCallbacks(
        std::function<void(const JointObject& src, JointObject& clone)> before,
        std::function<void(const JointObject& src, JointObject& clone)> after)
    {
        if(before != nullptr)
            m_beforeCopyCallbacks.emplace_back(before);

        if(after != nullptr)
            m_afterCopyCallbacks.emplace_back(before);
    }

protected:

    virtual void doCopyFrom(const JointObject& other)
    {
        m_world = other.m_world;
    }

    virtual void bindBodies(b2Body& bodyA, b2Body& bodyB) = 0;

    void create(const b2JointDef& jointDef)
    {
        if(getWorld() != nullptr)
            m_joint = getWorld()->CreateJoint(&jointDef);
    }

    b2Joint* getJoint() const
    {
        return m_joint;
    }

    b2World* getWorld() const
    {
        return m_world;
    }
};

#endif
