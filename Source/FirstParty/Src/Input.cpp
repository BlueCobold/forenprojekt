
#include "Input.hpp"
#include "TouchController.hpp"

#if defined(IOS)
#include "MacHelper.hpp"
#endif
#if defined(IOS) || defined(ANDROID)
#include <SFML/Window/Sensor.hpp>
#endif

namespace utility
{
    MouseWrapper Mouse;
    KeyboardWrapper Keyboard;
    TouchController TeeterController(sf::FloatRect(0, 0, 0, 0), 45.f, 45.f);

    void KeyboardWrapper::progress()
    {
        m_released.clear();
        m_newlyPressed.clear();
    }

    void KeyboardWrapper::notifyKeyPressed(const sf::Keyboard::Key key)
    {
        if(std::find(begin(m_stillPressed), end(m_stillPressed), key) == end(m_stillPressed))
        {
            m_newlyPressed.push_back(key);
            m_stillPressed.push_back(key);
        }
    }

    void KeyboardWrapper::notifyKeyReleased(const sf::Keyboard::Key key)
    {
        auto it = std::find(begin(m_stillPressed), end(m_stillPressed), key);
        if(it != end(m_stillPressed))
            m_stillPressed.erase(it);
        m_released.push_back(key);
    }

    bool KeyboardWrapper::isKeyDown(const sf::Keyboard::Key key)
    {
        return std::find(begin(m_newlyPressed), end(m_newlyPressed), key) != end(m_newlyPressed);
    }

    bool KeyboardWrapper::isKeyPressed(const sf::Keyboard::Key key)
    {
        return std::find(begin(m_stillPressed), end(m_stillPressed), key) != end(m_stillPressed);
    }

    bool KeyboardWrapper::isKeyReleased(const sf::Keyboard::Key key)
    {
        return std::find(begin(m_released), end(m_released), key) != end(m_released);
    }
    //===========================================================================================

    const sf::Vector2f& MouseWrapper::getPosition() const
    {
        return m_position;
    }

#if defined(IOS) || defined(ANDROID)
    void MouseWrapper::notifyTouch(const sf::Vector2i& pos)
    {
        m_touchPosition = pos;
    }
    
    const sf::Vector2i& MouseWrapper::getTouchPosition() const
    {
        return m_touchPosition;
    }
    
    const sf::Vector3f& MouseWrapper::getAcceleration() const
    {
        return m_currentAcceleration;
    }
#endif

    void MouseWrapper::startInterpolation(const sf::Window& relativeTo)
    {
        m_realWindowPosition = sf::Mouse::getPosition(relativeTo);
        m_totalPosition.x += m_realWindowPosition.x - relativeTo.getSize().x / 2;
        m_totalPosition.y += m_realWindowPosition.y - relativeTo.getSize().y / 2;
#ifndef TOUCHSIM
        sf::Mouse::setPosition(sf::Vector2i(relativeTo.getSize().x/2, relativeTo.getSize().y/2), relativeTo);
#endif
        TeeterController.startInterpolation();
    }

    void MouseWrapper::interpolate(int steps, int current)
    {
        auto percent = static_cast<float>(current) / steps;
        m_position = sf::Vector2f(
            m_lastPosition.x + (m_totalPosition.x - m_lastPosition.x) * percent,
            m_lastPosition.y + (m_totalPosition.y - m_lastPosition.y) * percent);
#if defined(IOS) || defined(ANDROID)
        m_currentAcceleration = m_lastAcceleration + (m_acceleration - m_lastAcceleration) * percent;
#endif
        TeeterController.interpolate(steps, current);
    }

    void MouseWrapper::capture()
    {
        m_lastPosition = m_totalPosition;
        m_position = sf::Vector2f(m_lastPosition);
        m_leftDown = m_isLeftDown && !m_leftPressed;
        m_leftPressed = m_isLeftDown;
        m_leftReleased = m_leftPreReleased;
        m_leftPreReleased = false;

        m_mouseWheelDown = false;
        m_mouseWheelUp = false;
        m_mouseWheelDelta = 0;

#if defined(IOS) || defined(ANDROID)
        if(m_sensorsEnabled && sf::Sensor::isAvailable(sf::Sensor::Accelerometer))
        {
            if(!m_sensorsInited)
            {
                sf::Sensor::setEnabled(sf::Sensor::Accelerometer, true);
                m_sensorsInited = true;
            }
            m_lastAcceleration = m_acceleration;
            m_acceleration = sf::Sensor::getValue(sf::Sensor::Accelerometer);
        }
#endif
    }

    void MouseWrapper::notifyButtonPressed(sf::Mouse::Button button)
    {
        if(button == sf::Mouse::Left)
            m_isLeftDown = true;
        else if(button == sf::Mouse::Right)
            m_isRightDown = true;
    }

    void MouseWrapper::notifyButtonReleased(sf::Mouse::Button button)
    {
        if(button == sf::Mouse::Left) {
            m_leftPreReleased = m_isLeftDown;
            m_isLeftDown = false;
        }
        else if(button == sf::Mouse::Right)
            m_isRightDown = false;
    }

    bool MouseWrapper::leftButtonPressed() const
    {
       return m_leftPressed;
    }

    bool MouseWrapper::leftButtonReleased() const
    {
        return m_leftReleased;
    }

    void  MouseWrapper::notifyWheelMoved(const int delta)
    {
        if(delta > 0)
        {
            m_mouseWheelUp = true;
            m_mouseWheelDown = false;
        }
        else if(delta < 0)
        {
            m_mouseWheelDown = true;
            m_mouseWheelUp = false;
        }
        m_mouseWheelDelta = delta;
    }

    bool MouseWrapper::isWheelMovedUp() const
    {
        return m_mouseWheelUp;
    }

    bool MouseWrapper::isWheelMovedDown() const
    {
        return m_mouseWheelDown;
    }

    int MouseWrapper::getWheelDelta() const
    {
        return m_mouseWheelDelta;
    }

    bool  MouseWrapper::leftButtonDown() const
    {
        return m_leftDown;
    }

    void MouseWrapper::hideSystemCursor(sf::RenderWindow& window)
    {
        m_cursorVisible = false;
    }

    void MouseWrapper::showSystemCursor(sf::RenderWindow& window)
    {
        m_cursorVisible = true;
    }

    bool MouseWrapper::isCursorVisible() const
    {
        return m_cursorVisible;
    }

#if defined(IOS) || defined(ANDROID)
    void MouseWrapper::enableSensors(bool enabled)
    {
        m_sensorsEnabled = enabled;
        m_sensorsInited = false;
    }
#endif

    const sf::Vector2i& MouseWrapper::getRealWindowPosition() const
    {
        return m_realWindowPosition;
    }
}

