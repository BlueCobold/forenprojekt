#pragma once

#include <string>

namespace Util
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
};