
#include "Input.hpp"

namespace utility
{
    MouseWrapper Mouse;
    KeyboardWrapper Keyboard;

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

    //===========================================================================================

    const sf::Vector2f& MouseWrapper::getPosition() const
    {
        return m_position;
    }

    void MouseWrapper::interpolate(int steps, int current)
    {
        m_position = sf::Vector2f(
            m_lastPosition.x + (sf::Mouse::getPosition().x - m_lastPosition.x) * static_cast<float>(current) / steps,
            m_lastPosition.y + (sf::Mouse::getPosition().y - m_lastPosition.y) * static_cast<float>(current) / steps);
    }

    void MouseWrapper::capture()
    {
        m_lastPosition = sf::Mouse::getPosition();
        m_position = sf::Vector2f(
            static_cast<float>(m_lastPosition.x),
            static_cast<float>(m_lastPosition.y));
        bool last = m_leftPressed;
        m_leftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        m_leftReleased = last && !m_leftPressed;
    }

    bool MouseWrapper::leftButtonPressed() const
    {
        return m_leftPressed;
    }

    bool MouseWrapper::leftButtonReleased() const
    {
        return m_leftReleased;
    }
}
