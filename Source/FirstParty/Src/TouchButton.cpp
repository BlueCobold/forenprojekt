#include "TouchButton.hpp"
#include "Input.hpp"

TouchButton::TouchButton(const sf::FloatRect& touchArea) :
    m_touchArea(touchArea),
    m_newlyPressed(false),
    m_stillPressed(false),
    m_released(false)
{ }

void TouchButton::update()
{
    if(m_touchArea.contains(sf::Vector2f(utility::Mouse.getRealWindowPosition())))
    {
        if(!m_stillPressed && !m_newlyPressed)
        {
            m_newlyPressed = true;
            m_stillPressed = true;
        }
        else if(m_stillPressed && m_newlyPressed)
            m_newlyPressed = false;
    }
    else
    {
        if(m_stillPressed && !m_released)
        {
            m_newlyPressed = false;
            m_stillPressed = false;
            m_released = true;
        }
        else if (m_released)
            m_released = false;
    }
}

bool TouchButton::isDown()
{
    return m_newlyPressed;
}

bool TouchButton::isPressed()
{
    return m_stillPressed;
}

bool TouchButton::isReleased()
{
    return m_released;
}

void TouchButton::setTouchArea(const sf::FloatRect& touchArea)
{
    m_touchArea = touchArea;
}