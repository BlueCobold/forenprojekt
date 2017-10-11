#include "Entity.hpp"
#include "Teeter.hpp"
#include "../Input.hpp"

#ifdef TOUCHSIM
#include "../TouchController.hpp"
#endif

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/Joints/b2RevoluteJoint.h>
#include <Box2D/Dynamics/b2Fixture.h>

#include <SFML/Window/Mouse.hpp>

#include <algorithm>
#include <cmath>

Teeter::Teeter(const float mouseScale, CloneHandler& cloneHandler) :
    Entity(Entity::Teeter, cloneHandler),
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

void Teeter::timeUpdated()
{
    const auto value = getPassedTime();
    auto velocity = 0.f;
    auto angle = utility::toDegree(getAngle());
    auto timeDiff = value - m_lastTime;

#if defined(IOS) || defined(ANDROID)
    auto gravToAngle = 90 / 9.81f;
    sf::Vector2f mousePos = 0.5f * sf::Vector2f(utility::Mouse.getAcceleration().y, utility::Mouse.getAcceleration().x)
                           +0.5f * m_lastMousePos;
    auto mouseDiff = sf::Vector2f((mousePos.x * gravToAngle - angle) / timeDiff,
                                  (mousePos.y * gravToAngle - angle) / timeDiff);
#else
#ifdef TOUCHSIM
    sf::Vector2f mousePos = sf::Vector2f(0, 0);
    if(m_useVerticalAxis)
        mousePos = sf::Vector2f(0, utility::TeeterController.getValue() * 6.f);
    else
        mousePos = sf::Vector2f(utility::TeeterController.getValue() * 6.f, 0);
#else
    sf::Vector2f mousePos = utility::Mouse.getPosition();
#endif

    sf::Vector2f mouseDiff = (m_lastMousePos - mousePos) * m_mouseScale / static_cast<float>(timeDiff * 60);
#endif

    auto minVelocity = static_cast<float>(((-45.f) - angle) / timeDiff);
    auto maxVelocity = static_cast<float>(((45.f) - angle) / timeDiff);

    if(m_invertAxis)
        mouseDiff = -mouseDiff;

    if(m_useVerticalAxis)
        maxVelocity = std::min(maxVelocity, std::max(minVelocity, mouseDiff.y));
    else
        maxVelocity = std::min(maxVelocity, std::max(minVelocity, mouseDiff.x));

    velocity = utility::toRadian(maxVelocity);

    getBody()->SetAngularVelocity(velocity);

    m_lastTime = value;
    m_lastMousePos = mousePos;
}

void Teeter::setControl(const bool invertAxis, const bool useVerticalAxis)
{
    m_invertAxis = invertAxis;
    m_useVerticalAxis = useVerticalAxis;
}