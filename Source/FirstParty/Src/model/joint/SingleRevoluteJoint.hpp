#pragma once

#ifndef SINGLE_REVOLUTE_JOINT_HPP
#define SINGLE_REVOLUTE_JOINT_HPP

#include "JointObject.hpp"
#include <Box2D/Dynamics/Joints/b2RevoluteJoint.h>
#include <Box2D/Dynamics/b2Body.h>

/// fix an Entity to his pivot point
class SingleRevoluteJoint : public JointObject
{
public:
    SingleRevoluteJoint(b2World& world, const b2RevoluteJointDef& jointDef, b2Body& body);
    SingleRevoluteJoint();

    void update() override;
    bool reinstall(b2Body& body) override;
    std::vector<b2Vec2> getAnchorPoints() const override;    
    std::vector<b2Vec2> getAnchorOffsets() const override;
    
private:
    b2RevoluteJointDef m_jointDef;
    b2Body* m_anchorBody;
    b2BodyDef m_anchorBodyDef;

    void bindBodies(b2Body& bodyA, b2Body& bodyB) override;
    void doCopyFrom(const JointObject& other) override;
};

#endif
