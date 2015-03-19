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
    auto velocity  = 0;
    auto angle = utility::toDegree<float,float>(getAngle());
    auto timeDiff = value - m_lastTime;

#ifdef IOS
    auto gravToAngle = 90 / 9.81f;
    sf::Vector2f mousePos = 0.5f * sf::Vector2f(utility::Mouse.getAcceleration().y, utility::Mouse.getAcceleration().x)
                           +0.5f * m_lastMousePos;
    auto mouseDiff = sf::Vector2f(mousePos.x * gravToAngle - angle,
                                  mousePos.y * gravToAngle - angle) / timeDiff;
#else
    sf::Vector2f mousePos = utility::Mouse.getPosition();
    sf::Vector2f mouseDiff = (m_lastMousePos - mousePos) * m_mouseScale / (timeDiff * 60);
#endif

    auto minVelocity = ((-45.f) - angle) / timeDiff;
    auto maxVelocity = ((45.f) - angle) / timeDiff;

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
void Teeter::setControl(const bool invertAxis, const bool useVerticalAxis)
{
    m_invertAxis = invertAxis;
    m_useVerticalAxis = useVerticalAxis;
}