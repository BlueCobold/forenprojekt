#include "Entity.hpp"
#include "Teeter.hpp"
#include "../Utility.hpp"

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/Joints/b2RevoluteJoint.h>
#include <Box2D/Dynamics/b2Fixture.h>

#include <SFML/Window/Mouse.hpp>

#include <algorithm>
#include <cmath>

Teeter::Teeter(const float mouseScale) : Entity(Entity::Teeter)
{
    m_lastMouseX = utility::Mouse.getPosition().x;
    m_lastTime = -1;

    m_mouseScale = mouseScale;
}

Teeter::~Teeter()
{
}

void Teeter::update(const float value)
{

    float velocity  = 0;
    float angle = utility::toDegree<float,float>(getAngle());
    float timeDiff = value - m_lastTime;

    float mouseX = utility::Mouse.getPosition().x;
    float mouseDiff = (m_lastMouseX - mouseX) * m_mouseScale / (timeDiff * 60);

    float minVelocity = ((-45.f) - angle) / timeDiff;   
    float maxVelocity = ((45.f) - angle) / timeDiff;
    
    maxVelocity = std::min(maxVelocity, std::max(minVelocity, mouseDiff));
    velocity = utility::toRadian<float, float>(maxVelocity);

    getBody()->SetAngularVelocity(velocity);
    
    m_lastTime = value;
    m_lastMouseX = mouseX;
    Entity::update(value);
}