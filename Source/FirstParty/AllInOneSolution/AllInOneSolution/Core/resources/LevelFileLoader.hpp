#pragma once

#ifndef LEVEL_FILE_LOADER_HPP
#define LEVEL_FILE_LOADER_HPP

#include "../Animation.hpp"
#include "../Entity.hpp"
#include "../animation/AngleCalculator.hpp"
#include "../animation/TimeCalculator.hpp"

#include <memory>

#include <tinyxml2.h>

class LevelFileLoader
{
public:

    static std::unique_ptr<ValueCalculator> parseTimedCalculator(
        tinyxml2::XMLElement* xml,
        const Entity* entity,
        const int frames);
        
    static std::unique_ptr<ValueCalculator> parseAngleCalculator(
        tinyxml2::XMLElement* xml,
        const Entity* entity,
        const int frames);

    static std::unique_ptr<Animation> parseAnimation(
        tinyxml2::XMLElement* xml,
        const Entity* entity);
};

#endif // LEVEL_FILE_LOADER_HPP
