#include "PhysicalObject.hpp"

PhysicalObject::PhysicalObject(float xPos, float yPos, float density, float friction, enum b2BodyType bodyType)
{
    m_body = 0;

    m_bodyDef.type = bodyType;
    m_bodyDef.position.Set(xPos, yPos);

    m_fixtureDef.density = density;
    m_fixtureDef.friction = friction;
}

PhysicalObject::~PhysicalObject()
{
    /// Pointer will be deletet by b2World
    m_body = 0;
}

void PhysicalObject::bindShape(b2CircleShape& shape)
{
    m_fixtureDef.shape = &shape;
}

void PhysicalObject::bindShape(b2PolygonShape& shape)
{
    m_fixtureDef.shape = &shape;
}

void PhysicalObject::bindShape(b2EdgeShape& shape)
{
    m_fixtureDef.shape = &shape;
}

void PhysicalObject::bindWorld(b2World& world)
{
    m_body = world.CreateBody(&m_bodyDef); 
    m_body->CreateFixture(&m_fixtureDef);
}