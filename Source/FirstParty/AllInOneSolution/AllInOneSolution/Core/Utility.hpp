#pragma once

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>
#include <sstream>

namespace utility
{
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
} // namespace utility

#endif // UTILITY_HPP
