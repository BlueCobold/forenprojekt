#include "Teeter.hpp"

#include "Utility.hpp"

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/Joints/b2RevoluteJoint.h>
#include <Box2D/Dynamics/b2Fixture.h>

#include <SFML/Window/Mouse.hpp>

Teeter::Teeter(const float32 x, const float32 y, const float centerX, const float centerY,
    const b2FixtureDef& fixtureDef, b2World& world)
{
    m_lastMouseX = sf::Mouse::getPosition().x;

    	b2BodyDef bodyDef;

        // Create teeter

        bodyDef.position.Set(x, y);
        bodyDef.type = b2_kinematicBody;

        m_body = world.CreateBody( &bodyDef );

        m_body->CreateFixture( &fixtureDef );
}

Teeter::~Teeter()
{
}

void Teeter::update(const float value)
{
    float angle = utility::toDegree<float32,float>(m_body->GetAngle());

    int mouseX = sf::Mouse::getPosition().x;
    float mouseDiff = (m_lastMouseX-mouseX)/(-10.0f);

    if(angle < -45.f)
    {
        if (mouseDiff<0)
            m_body->SetAngularVelocity(0.f);
        else
            m_body->SetAngularVelocity(mouseDiff);
    }
    else
    {
        if(angle > 45.f && mouseDiff>0)
            m_body->SetAngularVelocity(0.f);
        else
            m_body->SetAngularVelocity(mouseDiff);
    }
    m_lastMouseX = mouseX;
    Entity::update(value);
}