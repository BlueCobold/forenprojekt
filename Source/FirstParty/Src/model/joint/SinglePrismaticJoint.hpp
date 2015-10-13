#pragma once

#ifndef SINGLE_PRISMATIC_JOINT_HPP
#define SINGLE_PRISMATIC_JOINT_HPP

#include "JointObject.hpp"
#include <Box2D/Dynamics/Joints/b2PrismaticJoint.h>
#include <Box2D/Dynamics/b2Body.h>

class SinglePrismaticJoint : public JointObject
{
private:
    b2PrismaticJointDef m_jointDef;

    b2Body* m_anchorBody;
    b2BodyDef m_anchorBodyDef;

    b2Vec2 m_direction;

public:
    SinglePrismaticJoint(b2World* world, const b2PrismaticJointDef& jointDef, b2Body* body, const b2Vec2& direction);

    SinglePrismaticJoint();

    void update() override;

    bool reinstall(b2Body* body) override;

    void copyFrom(const JointObject* other) override;

    std::vector<b2Vec2> getAnchorPoints();

protected:
    void bindBodies(b2Body* bodyA, b2Body* bodyB) override;
};

#endif
