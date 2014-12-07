#include "SingleRevoluteJoint.hpp"
#include "../../Utility.hpp"

SingleRevoluteJoint::SingleRevoluteJoint(b2World* world, const b2RevoluteJointDef jointDef, b2Body* body) :
    JointObject(world, JointObject::SingleRevolute),
    m_jointDef(jointDef)
{
    m_anchorBodyDef.type = b2_staticBody;
    m_anchorBody = world->CreateBody(&m_anchorBodyDef);

    if(body->GetType() == b2_dynamicBody)
    {
        bindBodys(body, m_anchorBody);
        m_jointDef.Initialize(body, m_anchorBody, body->GetPosition() + m_jointDef.localAnchorA);
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("WrongBodyType"), "SingleRevoluteJoint"));

    getJoint() = static_cast<b2RevoluteJoint*>(JointObject::getWorld()->CreateJoint(&m_jointDef));
}

SingleRevoluteJoint::~SingleRevoluteJoint()
{
    if(getJoint() != nullptr)
        getWorld()->DestroyJoint(getJoint());

    if(m_anchorBody != nullptr)
        JointObject::getWorld()->DestroyBody(m_anchorBody);

    m_anchorBody = nullptr;
}

void SingleRevoluteJoint::bindBodys(b2Body* bodyA, b2Body* bodyB)
{
    m_jointDef.bodyA = bodyA;
    m_jointDef.bodyB = bodyB;
}