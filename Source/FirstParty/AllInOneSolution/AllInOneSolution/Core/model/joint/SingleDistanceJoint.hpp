#pragma once

#ifndef SINGLE_DISTANCE_JOINT_HPP
#define SINGLE_DISTANCE_JOINT_HPP

#include "JointObject.hpp"
#include <Box2D/Dynamics/Joints/b2DistanceJoint.h>
#include <Box2D/Dynamics/b2Body.h>

class SingleDistanceJoint : public JointObject
{
private:
    b2DistanceJointDef m_jointDef;

    b2Body* m_anchorBody;
    b2BodyDef m_anchorBodyDef;

public:
    SingleDistanceJoint(b2World* world, const b2DistanceJointDef& jointDef, b2Body* body);

    ~SingleDistanceJoint();

    void update() override;

protected:
    void bindBodys(b2Body* bodyA, b2Body* bodyB) override;
};

#endif