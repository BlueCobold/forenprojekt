#include "SinglePrismaticJoint.hpp"
#include "../../Utility.hpp"
#include <cmath>

SinglePrismaticJoint::SinglePrismaticJoint(b2World* world, const b2PrismaticJointDef& jointDef,b2Body* body, float towards) :
    JointObject(world, JointObject::SinglePrismatic),
    m_jointDef(jointDef)
{
    m_anchorBodyDef.type = b2_staticBody;
    m_anchorBody = world->CreateBody(&m_anchorBodyDef);

    if(body->GetType() == b2_dynamicBody)
    {
        bindBodys(body, m_anchorBody);
        m_jointDef.Initialize(body, m_anchorBody, body->GetWorldCenter(), b2Vec2(std::cos(towards), std::sin(towards)));
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("WrongBodyType"), "SinglePrismaticJoint"));

    getJoint() = static_cast<b2PrismaticJoint*>(JointObject::getWorld()->CreateJoint(&m_jointDef));
}

SinglePrismaticJoint::~SinglePrismaticJoint()
{
    m_anchorBody = nullptr;
}

void SinglePrismaticJoint::bindBodys(b2Body* bodyA, b2Body* bodyB)
{
    m_jointDef.bodyA = bodyA;
    m_jointDef.bodyB = bodyB;
}

void SinglePrismaticJoint::update()
{
    
}