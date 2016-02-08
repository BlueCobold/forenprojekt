#pragma once

#ifndef INPUT_HPP
#define INPUT_HPP

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#if defined(IOS) || defined(ANDROID)
#include <SFML/System/Vector3.hpp>
#endif

#include <algorithm>
#include <vector>

namespace utility
{
    class KeyboardWrapper
    {
    public:
        KeyboardWrapper()
        { }

        void progress();

        void notifyKeyPressed(const sf::Keyboard::Key key);

        void notifyKeyReleased(const sf::Keyboard::Key key);

        bool isKeyDown(const sf::Keyboard::Key key);

        bool isKeyPressed(const sf::Keyboard::Key key);

        bool isKeyReleased(const sf::Keyboard::Key key);

    private:
        std::vector<sf::Keyboard::Key> m_newlyPressed;
        std::vector<sf::Keyboard::Key> m_stillPressed;
        std::vector<sf::Keyboard::Key> m_released;
    };
    extern KeyboardWrapper Keyboard;

    class MouseWrapper
    {
    public:
        MouseWrapper() : m_leftPressed(false),
#if defined(IOS) || defined(ANDROID)
            m_sensorsEnabled(false),
#endif
            m_leftDown(false)
        {
            capture();
        }

        const sf::Vector2f& getPosition() const;
        sf::Vector2i getMoveDistanze() const;
        void startInterpolation(const sf::Window& relativeTo);
        void interpolate(int steps, int current);
#if defined(IOS) || defined(ANDROID)
        void notifyTouch(const sf::Vector2i& pos);
        const sf::Vector2i& getTouchPosition() const;
        const sf::Vector3f& getAcceleration() const;
#endif
        void capture();

        void notifyButtonPressed(sf::Mouse::Button button);
        void notifyButtonReleased(sf::Mouse::Button button);

        bool leftButtonPressed() const;

        bool leftButtonReleased() const;

        bool leftButtonDown() const;

        void notifyWheelMoved(const int delta);

        bool isWheelMovedUp() const;

        bool isWheelMovedDown() const;

        void hideSystemCursor(sf::RenderWindow& window);
        void showSystemCursor(sf::RenderWindow& window);

        bool isCursorVisible() const;

#if defined(IOS) || defined(ANDROID)
        void enableSensors(bool enabled);
#endif

    private:
#if defined(IOS) || defined(ANDROID)
        sf::Vector2i m_touchPosition;
        sf::Vector3f m_lastAcceleration;
        sf::Vector3f m_currentAcceleration;
        sf::Vector3f m_acceleration;
        bool m_sensorsEnabled;
        bool m_sensorsInited;
#endif
        bool m_cursorVisible;
        bool m_leftPressed;
        bool m_leftDown;
        bool m_leftReleased;
        bool m_isLeftDown; // tracks the physical state of the left mouse button
        bool m_isRightDown; // tracks the physical state of the right mouse button
        bool m_mouseWheelUp;
        bool m_mouseWheelDown;
        sf::Vector2f m_position;
        sf::Vector2i m_lastPosition;
        sf::Vector2i m_totalPosition;
        sf::Vector2i m_beforMovePosition;
    };
    extern MouseWrapper Mouse;
} // namespace utility

#endif // INPUT_HPP
