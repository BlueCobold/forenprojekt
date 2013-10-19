#include "Entity.hpp"
#include "Teeter.hpp"
#include "../Input.hpp"

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/Joints/b2RevoluteJoint.h>
#include <Box2D/Dynamics/b2Fixture.h>

#include <SFML/Window/Mouse.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>

Teeter::Teeter(const float mouseScale) :
    Entity(Entity::Teeter),
    m_lastMousePos(utility::Mouse.getPosition()),
    m_lastTime(-1),
    m_mouseScale(mouseScale),
    m_invertAxis(false),
    m_useVerticalAxis(false)
{ }

Teeter::~Teeter()
{
}

void Teeter::adaptToMouse()
{
    m_lastMousePos = utility::Mouse.getPosition();
}

void Teeter::update(const float value)
{
    float velocity  = 0;
    float angle = utility::toDegree<float,float>(getAngle());
    float timeDiff = value - m_lastTime;

    sf::Vector2f mousePos = utility::Mouse.getPosition();
    sf::Vector2f mouseDiff = (m_lastMousePos - mousePos) * m_mouseScale / (timeDiff * 60);

    float minVelocity = ((-45.f) - angle) / timeDiff;   
    float maxVelocity = ((45.f) - angle) / timeDiff;

    if(m_invertAxis)
        mouseDiff = -mouseDiff;

    if(m_useVerticalAxis)
        maxVelocity = std::min(maxVelocity, std::max(minVelocity, mouseDiff.y));
    else
        maxVelocity = std::min(maxVelocity, std::max(minVelocity, mouseDiff.x));

    velocity = utility::toRadian<float, float>(maxVelocity);

    getBody()->SetAngularVelocity(velocity);

    m_lastTime = value;
    m_lastMousePos = mousePos;
    Entity::update(value);
}
void Teeter::setControll(const bool invertAxis, const bool useVerticalAxis)
{
    m_invertAxis = invertAxis;
    m_useVerticalAxis = useVerticalAxis;
}