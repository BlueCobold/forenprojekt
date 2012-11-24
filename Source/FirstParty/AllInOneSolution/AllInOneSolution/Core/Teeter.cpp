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
    	b2BodyDef bodyDef;
        b2FixtureDef fixDef;
        b2PolygonShape polyShape;

        b2Body* binder;

        b2RevoluteJointDef revDef;

        // Create binder

        polyShape.SetAsBox(1,1);

        bodyDef.type = b2_staticBody;
        bodyDef.position.Set(x, y);

        fixDef.density = 0.1f;
        fixDef.friction = 0.1f;
        fixDef.shape = &polyShape;

        binder = world.CreateBody(&bodyDef);

        binder->CreateFixture(&fixDef);

        // Create teeter

        bodyDef.position.Set(x, y);
        bodyDef.type = b2_dynamicBody;

        m_body = world.CreateBody( &bodyDef );

        m_body->CreateFixture( &fixtureDef );

        // joints

        revDef.bodyA = binder;
        revDef.bodyB = m_body;
        revDef.localAnchorA.Set( centerX, centerY);
        revDef.localAnchorB.Set( centerX, centerY);

        revDef.enableLimit = true;
        revDef.lowerAngle = utility::toRadian<double,float32>(-45.0);
        revDef.upperAngle = utility::toRadian<double,float32>(45.0);

        revDef.collideConnected = false;

        world.CreateJoint( &revDef );

}

Teeter::~Teeter()
{
}

void Teeter::update(const float value)
{
    Entity::update(value);

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        m_body->ApplyAngularImpulse(5, false);
    if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
        m_body->ApplyAngularImpulse(-5, false);

}