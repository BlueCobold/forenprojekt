#pragma once

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <Box2D/Common/b2Math.h>

#include <algorithm>
#include <vector>
#include <sstream>
#include <string>

namespace utility
{
    static const float PIXEL_PER_METER = 10.f;

    static const std::string VERSION = "0.8";

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
        std::stringstream stringstream(str);
        
        T value;
        stringstream >> value;
        if(stringstream.fail())
        {
            auto str2 = str;
            // clang/MacOS doesn't like a trailing "f" in floats...
            stringstream = std::stringstream(str2.replace(str.find("f"), 1, ""));
            stringstream >> value;
        }
        
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

    b2Vec2 rotate(const b2Vec2& v, float angle);

    int hexToInt(const std::string& hex);

    // "RRGGBBAA"
    sf::Color hexToColor(const std::string& hex);

    std::string translateKey(std::string key);

    std::string replace(std::string string, std::string replacement);

    sf::View getDefaultView(const sf::RenderTarget& target, const sf::Vector2f viewSize);
    sf::View getDefaultView(const sf::RenderTarget& target, const sf::Vector2u viewSize);

    struct Event
    {
        enum EventType
        {
            NoEvent,
            LostFocus,
            GainFocus,
            Resized
        };
        EventType m_eventType;
    };
} // namespace utility

#endif // UTILITY_HPP
