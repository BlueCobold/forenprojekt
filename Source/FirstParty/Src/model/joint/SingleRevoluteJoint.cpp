#include "SingleRevoluteJoint.hpp"
#include "../../Utility.hpp"

SingleRevoluteJoint::SingleRevoluteJoint(b2World& world, const b2RevoluteJointDef& jointDef, b2Body& body) :
    JointObject(world, JointObject::SingleRevolute),
    m_jointDef(jointDef)
{
    m_anchorBodyDef.type = b2_staticBody;
    m_anchorBody = world.CreateBody(&m_anchorBodyDef);

    reinstall(body);
}

SingleRevoluteJoint::SingleRevoluteJoint() :
    JointObject(JointObject::SingleRevolute),
    m_anchorBody(nullptr)
{ }

void SingleRevoluteJoint::bindBodies(b2Body& bodyA, b2Body& bodyB)
{
    m_jointDef.bodyA = &bodyA;
    m_jointDef.bodyB = &bodyB;
}

bool SingleRevoluteJoint::reinstall(b2Body& body)
{
    if(getWorld() && m_anchorBody)
    {
        if(body.GetType() == b2_dynamicBody)
        {
            bindBodies(body, *m_anchorBody);
            m_jointDef.Initialize(&body, m_anchorBody, body.GetPosition() + m_jointDef.localAnchorA);
        }
        else
            throw std::runtime_error(utility::replace(utility::translateKey("@WrongBodyType"), "SingleRevoluteJoint"));

        create(m_jointDef);

        return true;
    }
    return false;
}

void SingleRevoluteJoint::update()
{
}

void SingleRevoluteJoint::doCopyFrom(const JointObject& other)
{
    if(getType() != other.getType())
        throw std::runtime_error(utility::translateKey("@JointCopyError"));

    auto joint = dynamic_cast<const SingleRevoluteJoint&>(other);

    JointObject::copyFrom(joint);

    m_anchorBodyDef = joint.m_anchorBodyDef;

    if(getWorld() != nullptr)
        m_anchorBody = getWorld()->CreateBody(&m_anchorBodyDef);
}

std::vector<b2Vec2> SingleRevoluteJoint::getAnchorPoints() const
{
    std::vector<b2Vec2> anchors;
    if(auto joint = getJoint())
        anchors.push_back(joint->GetAnchorA());
    return anchors;
}

std::vector<b2Vec2> SingleRevoluteJoint::getAnchorOffsets() const
{
    std::vector<b2Vec2> anchors;
    if(auto joint = getJoint())
        if(auto bodyA = joint->GetBodyA())
            anchors.push_back(joint->GetAnchorA() - bodyA->GetPosition());

    return anchors;
}
