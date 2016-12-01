#pragma once

#ifndef BLENDING_PARSER_HPP
#define BLENDING_PARSER_HPP

#include "../rendering/Blending.hpp"

#include <tinyxml2.h>

#include <string>

inline Blending::Mode parseBlending(const tinyxml2::XMLElement& xml)
{
    auto mode = Blending::RegularAlpha;
    if(auto blend = xml.Attribute("blending"))
    {
        if(std::string("add") == blend)
            mode = Blending::Add;
        else if(std::string("mul") == blend)
            mode = Blending::Multiply;
        else if(std::string("premul") == blend)
            mode = Blending::PreMultipliedAlpha;
    }
    return mode;
}

#endif // BLENDING_PARSER_HPP
