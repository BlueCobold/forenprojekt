#include "SingleRevoluteJoint.hpp"
#include "../../Utility.hpp"

SingleRevoluteJoint::SingleRevoluteJoint(b2World* world, const b2RevoluteJointDef& jointDef, b2Body* body) :
    JointObject(world, JointObject::SingleRevolute),
    m_jointDef(jointDef)
{
    m_anchorBodyDef.type = b2_staticBody;
    m_anchorBody = world->CreateBody(&m_anchorBodyDef);

    if(body->GetType() == b2_dynamicBody)
    {
        bindBodies(body, m_anchorBody);
        m_jointDef.Initialize(body, m_anchorBody, body->GetPosition() + m_jointDef.localAnchorA);
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("WrongBodyType"), "SingleRevoluteJoint"));

    getJoint() = static_cast<b2RevoluteJoint*>(JointObject::getWorld()->CreateJoint(&m_jointDef));
}

SingleRevoluteJoint::SingleRevoluteJoint() :
    JointObject(JointObject::SingleRevolute),
    m_anchorBody(nullptr)
{
}

SingleRevoluteJoint::~SingleRevoluteJoint()
{
    m_anchorBody = nullptr;
}

void SingleRevoluteJoint::bindBodies(b2Body* bodyA, b2Body* bodyB)
{
    m_jointDef.bodyA = bodyA;
    m_jointDef.bodyB = bodyB;
}

bool SingleRevoluteJoint::reinstall(b2Body* body)
{
    if(getWorld() && body && m_anchorBody)
    {
        if(body->GetType() == b2_dynamicBody)
        {
            bindBodies(body, m_anchorBody);
            m_jointDef.Initialize(body, m_anchorBody, body->GetPosition() + m_jointDef.localAnchorA);
        }
        else
            throw std::runtime_error(utility::replace(utility::translateKey("WrongBodyType"), "SingleRevoluteJoint"));

        getJoint() = static_cast<b2RevoluteJoint*>(JointObject::getWorld()->CreateJoint(&m_jointDef));

        return true;
    }
    return false;
}

void SingleRevoluteJoint::update()
{
}

void SingleRevoluteJoint::copyFrom(const JointObject* other)
{
    if(getType() != other->getType())
        throw std::runtime_error(utility::translateKey("JointCopyError"));

    const SingleRevoluteJoint* joint = dynamic_cast<const SingleRevoluteJoint*>(other);

    JointObject::copyFrom(joint);

    m_anchorBodyDef = joint->m_anchorBodyDef;

    m_anchorBody = getWorld()->CreateBody(&m_anchorBodyDef);
}