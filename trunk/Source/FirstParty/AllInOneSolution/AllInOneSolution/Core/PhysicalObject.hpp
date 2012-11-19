#pragma once

#ifndef PHYSICAL_OBJECT_HPP
#define PHYSICAL_OBJECT_HPP

#include <Box2D/Box2D.h>

/// This class will be used to give a object a binding to
/// a physical body
class PhysicalObject
{
public:

    /// Default Constructor extra for Entity class
    PhysicalObject()
    {
        m_body = NULL;
    }
    PhysicalObject(enum b2BodyType bodyType, float xPos, float yPos, float density, float friction)
    {
        m_body = NULL;

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

    void bindToWorld(b2World world)
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
