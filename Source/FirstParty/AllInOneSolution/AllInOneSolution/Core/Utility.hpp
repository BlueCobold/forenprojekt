#pragma once

#ifndef UTILITY_HPP
#define UTILITY_HPP

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
    T stringTo(const std::string& string)
    {
        T value;

        std::stringstream stringstream(string);

        stringstream >> value;

        return value;
    }

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
} // namespace utility

#endif // UTILITY_HPP
