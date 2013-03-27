#pragma once

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <algorithm>
#include <vector>
#include <sstream>
#include <string>

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

    int hexToInt(const std::string& hex);

    // "RRGGBBAA"
    sf::Color hexToColor(const std::string& hex);

    std::string translateKey(std::string key);

    std::string replace(std::string string, std::string replacement);

} // namespace utility

#endif // UTILITY_HPP
