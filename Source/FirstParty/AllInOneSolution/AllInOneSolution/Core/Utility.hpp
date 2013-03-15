#pragma once

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>

#include <string>
#include <sstream>

namespace utility
{
    static const float PIXEL_PER_METER = 10.f;

    template<typename T>
    std::string toString(const T& value)
    {
        std::ostringstream out;

        out << value;
    
        return out.str();
    }

    template<typename T>
    T stringTo(const std::string& str)
    {
        T value;

        std::stringstream stringstream(str);

        stringstream >> value;

        return value;
    }

	template<>
    std::string stringTo(const std::string& str);

    template<typename T, typename Z>
    Z toDegree(T value)
    {
        value *= 57.295779513082320876f;
        return (Z)value;
    }

    template<typename T, typename Z>
    Z toRadian(T value)
    {
        value *= 0.0174532925199432957f;
        return (Z)value;
    }

    template<typename T>
    T toMeter(T value)
    {
        return static_cast<T>(value/PIXEL_PER_METER);
    }

    template<typename T>
    T toPixel(T value)
    {
        return static_cast<T>(value*PIXEL_PER_METER);
    }

    std::string translateKey(std::string key);

    std::string replace(std::string string, std::string replacement);

    class MouseWrapper
    {
    public:
        MouseWrapper()
        {
            capture();
        }

        const sf::Vector2f& getPosition() const
        {
            return m_position;
        }

        void interpolate(int steps, int current)
        {
            m_position = sf::Vector2f(
                m_lastPosition.x + (sf::Mouse::getPosition().x - m_lastPosition.x) * static_cast<float>(current) / steps,
                m_lastPosition.y + (sf::Mouse::getPosition().y - m_lastPosition.y) * static_cast<float>(current) / steps);
        }

        void capture()
        {
            m_lastPosition = sf::Mouse::getPosition();
            m_position = sf::Vector2f(
                    static_cast<float>(m_lastPosition.x),
                    static_cast<float>(m_lastPosition.y));
        }

    private:
        
        sf::Vector2f m_position;
        sf::Vector2i m_lastPosition;
    };
    extern MouseWrapper Mouse;
} // namespace utility

#endif // UTILITY_HPP
