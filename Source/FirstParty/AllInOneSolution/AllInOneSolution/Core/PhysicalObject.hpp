#pragma once

#ifndef PHYSICAL_OBJECT_HPP
#define PHYSICAL_OBJECT_HPP

#include <Box2D/Box2D.h>

/// This class will be used to give a object a binding to
/// a physical body
class PhysicalObject
{
public:
    /// Same as a default Constructor
    PhysicalObject(float xPos = 0.0f, float yPos = 0.0f, 
                   float density = 0.0f, float friction = 0.0f, 
                   enum b2BodyType bodyType = b2_staticBody)
    {
        m_body = 0;

        m_bodyDef.type = bodyType;
        m_bodyDef.position.Set(xPos, yPos);

        m_fixtureDef.density = density;
        m_fixtureDef.friction = friction;
    }

    virtual ~PhysicalObject(){}

    /// Overloaded Function this is for a b2CircleShape
    void bindShape(b2CircleShape& shape)
    {
        m_fixtureDef.shape = &shape;
    }
    /// Overloaded Function this is for a b2PolygonShape
    void bindShape(b2PolygonShape shape)
    {
        m_fixtureDef.shape = &shape;
    }
    /// Overloaded Function this is for a b2EdgeShape
    void bindShape(b2EdgeShape shape)
    {
        m_fixtureDef.shape = &shape;
    }

    void bindWorld(b2World world)
    {
        m_body = world.CreateBody(&m_bodyDef); 
        m_body->CreateFixture(&m_fixtureDef);
    }
protected:

    b2Body* m_body;
    b2BodyDef m_bodyDef;
    b2FixtureDef m_fixtureDef;
};

#endif // PHYSICAL_OBJECT_HPP
