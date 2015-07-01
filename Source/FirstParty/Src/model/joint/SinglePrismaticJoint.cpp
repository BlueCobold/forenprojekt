#include "SinglePrismaticJoint.hpp"
#include "../../Utility.hpp"

SinglePrismaticJoint::SinglePrismaticJoint(b2World* world, const b2PrismaticJointDef& jointDef, b2Body* body, const b2Vec2& direction) :
    JointObject(world, JointObject::SinglePrismatic),
    m_jointDef(jointDef),
    m_direction(direction)
{
    m_anchorBodyDef.type = b2_staticBody;
    m_anchorBody = world->CreateBody(&m_anchorBodyDef);

    if(body->GetType() == b2_dynamicBody)
    {
        bindBodies(body, m_anchorBody);
        m_jointDef.Initialize(body, m_anchorBody, body->GetWorldCenter(), m_direction);
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("WrongBodyType"), "SinglePrismaticJoint"));

    getJoint() = static_cast<b2PrismaticJoint*>(JointObject::getWorld()->CreateJoint(&m_jointDef));
}

SinglePrismaticJoint::SinglePrismaticJoint() :
    JointObject(JointObject::SinglePrismatic),
    m_anchorBody(nullptr)
{ }

SinglePrismaticJoint::~SinglePrismaticJoint()
{
    m_anchorBody = nullptr;
}

void SinglePrismaticJoint::bindBodies(b2Body* bodyA, b2Body* bodyB)
{
    m_jointDef.bodyA = bodyA;
    m_jointDef.bodyB = bodyB;
}

bool SinglePrismaticJoint::reinstall(b2Body* body)
{
    if(getWorld() && body && m_anchorBody)
    {
        if(body->GetType() == b2_dynamicBody)
        {
            bindBodies(body, m_anchorBody);
            m_jointDef.Initialize(body, m_anchorBody, body->GetWorldCenter(), m_direction);
        }
        else
            throw std::runtime_error(utility::replace(utility::translateKey("WrongBodyType"), "SinglePrismaticJoint"));

        getJoint() = static_cast<b2PrismaticJoint*>(JointObject::getWorld()->CreateJoint(&m_jointDef));

        return true;
    }
    return false;
}

void SinglePrismaticJoint::update()
{ }

void SinglePrismaticJoint::copyFrom(const JointObject* other)
{
    if(getType() != other->getType())
        throw std::runtime_error(utility::translateKey("JointCopyError"));

    const SinglePrismaticJoint* joint = dynamic_cast<const SinglePrismaticJoint*>(other);

    JointObject::copyFrom(joint);

    m_anchorBodyDef = joint->m_anchorBodyDef;
    m_direction = joint->m_direction;

    m_anchorBody = getWorld()->CreateBody(&m_anchorBodyDef);
}