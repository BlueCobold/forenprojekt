#include "SingleDistanceJoint.hpp"
#include "../../Utility.hpp"

SingleDistanceJoint::SingleDistanceJoint(b2World& world, const b2DistanceJointDef& jointDef, b2Body& body) :
    JointObject(world, JointObject::SingleDistance),
    m_jointDef(jointDef)
{
    m_anchorBodyDef.type = b2_staticBody;
    m_anchorBody = world.CreateBody(&m_anchorBodyDef);

    reinstall(body);
}

SingleDistanceJoint::SingleDistanceJoint() :
    JointObject(JointObject::SingleDistance),
    m_anchorBody(nullptr)
{ }

void SingleDistanceJoint::bindBodies(b2Body& bodyA, b2Body& bodyB)
{
    m_jointDef.bodyA = &bodyA;
    m_jointDef.bodyB = &bodyB;
}

bool SingleDistanceJoint::reinstall(b2Body& body)
{
    if(body.GetType() == b2_dynamicBody)
    {
        bindBodies(body, *m_anchorBody);
        m_jointDef.Initialize(&body, m_anchorBody, body.GetPosition() + m_jointDef.localAnchorA, body.GetPosition() + m_jointDef.localAnchorB);
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("WrongBodyType"), "SingleRevoluteJoint"));

    create(m_jointDef);
    return true;
}

void SingleDistanceJoint::update()
{
}

void SingleDistanceJoint::doCopyFrom(const JointObject& other)
{
    if(getType() != other.getType())
        throw std::runtime_error(utility::translateKey("JointCopyError"));

    auto joint = dynamic_cast<const SingleDistanceJoint&>(other);

    JointObject::copyFrom(joint);

    m_anchorBodyDef = joint.m_anchorBodyDef;
    
    if(getWorld() != nullptr)
        m_anchorBody = getWorld()->CreateBody(&m_anchorBodyDef);
}

std::vector<b2Vec2> SingleDistanceJoint::getAnchorPoints() const
{
    std::vector<b2Vec2> anchors;
    if(auto joint = getJoint())
    {
        anchors.push_back(joint->GetAnchorA());
        anchors.push_back(joint->GetAnchorB());
    }
    return anchors;
}

std::vector<b2Vec2> SingleDistanceJoint::getAnchorOffsets() const
{
    std::vector<b2Vec2> anchors;
    if(auto joint = getJoint())
    {
        if(auto bodyA = joint->GetBodyA())
            anchors.push_back(joint->GetAnchorA() - bodyA->GetPosition());
        anchors.push_back(b2Vec2(0, 0));
    }
    return anchors;
}
