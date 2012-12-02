#include "Teeter.hpp"

#include "Utility.hpp"

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/Joints/b2RevoluteJoint.h>
#include <Box2D/Dynamics/b2Fixture.h>

#include <SFML/Window/Mouse.hpp>

#include <algorithm>
#include <cmath>

Teeter::Teeter(const float32 x, const float32 y, const float centerX, const float centerY,
    const b2FixtureDef& fixtureDef, b2World& world, const float mouseScale)
{
    m_lastMouseX = sf::Mouse::getPosition().x;
    m_lastTime = -1;
    b2BodyDef bodyDef;

    m_mouseScale = mouseScale;

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

    float velocity  = 0;
    float angle = utility::toDegree<float,float>(m_body->GetAngle());
    float timeDiff = value - m_lastTime;

    int mouseX = sf::Mouse::getPosition().x;
    float mouseDiff = (m_lastMouseX - mouseX) * m_mouseScale / (timeDiff * 60);

    float minVelocity = ((-45.f) - angle) / timeDiff;   
    float maxVelocity = ((45.f) - angle) / timeDiff;
    
    maxVelocity = std::min(maxVelocity, std::max(minVelocity, mouseDiff));
    velocity = utility::toRadian<float,float>(maxVelocity);
   
    m_body->SetAngularVelocity(velocity);
    
    m_lastTime = value;
    m_lastMouseX = mouseX;
    Entity::update(value);
}