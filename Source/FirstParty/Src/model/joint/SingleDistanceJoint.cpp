#include "SingleDistanceJoint.hpp"
#include "../../Utility.hpp"

SingleDistanceJoint::SingleDistanceJoint(b2World* world, const b2DistanceJointDef& jointDef, b2Body* body) :
    JointObject(world, JointObject::SingleDistance),
    m_jointDef(jointDef)
{
    m_anchorBodyDef.type = b2_staticBody;
    m_anchorBody = world->CreateBody(&m_anchorBodyDef);

    reinstall(body);
}

SingleDistanceJoint::SingleDistanceJoint() :
    JointObject(JointObject::SingleDistance),
    m_anchorBody(nullptr)
{ }

void SingleDistanceJoint::bindBodies(b2Body* bodyA, b2Body* bodyB)
{
    m_jointDef.bodyA = bodyA;
    m_jointDef.bodyB = bodyB;
}

bool SingleDistanceJoint::reinstall(b2Body* body)
{
    if(getWorld() && body && m_anchorBody)
    {
        if(body->GetType() == b2_dynamicBody)
        {
            bindBodies(body, m_anchorBody);
            m_jointDef.Initialize(body, m_anchorBody, body->GetPosition() + m_jointDef.localAnchorA, body->GetPosition() + m_jointDef.localAnchorB);
        }
        else
            throw std::runtime_error(utility::replace(utility::translateKey("WrongBodyType"), "SingleRevoluteJoint"));

        create(m_jointDef);
        return true;
    }

    return false;
}

void SingleDistanceJoint::update()
{
}

void SingleDistanceJoint::copyFrom(const JointObject* other)
{
    if(getType() != other->getType())
        throw std::runtime_error(utility::translateKey("JointCopyError"));

    const SingleDistanceJoint* joint = dynamic_cast<const SingleDistanceJoint*>(other);

    JointObject::copyFrom(joint);

    m_anchorBodyDef = joint->m_anchorBodyDef;

    m_anchorBody = getWorld()->CreateBody(&m_anchorBodyDef);
}

std::vector<b2Vec2> SingleDistanceJoint::getAnchorPoints()
{
    std::vector<b2Vec2> anchors;
    if(auto joint = getJoint())
    {
        anchors.push_back(getJoint()->GetAnchorA());
        anchors.push_back(getJoint()->GetAnchorB());
    }
    return anchors;
}
