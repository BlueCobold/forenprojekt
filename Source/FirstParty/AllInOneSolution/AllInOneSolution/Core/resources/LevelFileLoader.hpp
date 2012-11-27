#pragma once

#ifndef LEVEL_FILE_LOADER_HPP
#define LEVEL_FILE_LOADER_HPP

#include "../Animation.hpp"
#include "../Entity.hpp"
#include "../AnimatedGraphics.hpp"
#include "../animation/ValueProvider.hpp"

#include "ResourceManager.hpp"

#include <memory>

#include <tinyxml2.h>

class LevelFileLoader
{
public:

    static std::unique_ptr<ValueCalculator> parseStaticCalculator(
        tinyxml2::XMLElement* xml);

    static std::unique_ptr<ValueCalculator> parseTimedCalculator(
        tinyxml2::XMLElement* xml,
        const AnimatedGraphics* entity,
        const int frames);
        
    static std::unique_ptr<ValueCalculator> parseAngleCalculator(
        tinyxml2::XMLElement* xml,
        const Entity* entity,
        const int frames);

     static std::unique_ptr<Animation> parseAnimation(
         tinyxml2::XMLElement* xml,
         const AnimatedGraphics* animated,
         ResourceManager& resourceManager);

    static std::unique_ptr<Animation> parseAnimation(
        tinyxml2::XMLElement* xml,
        const Entity* entity,
        ResourceManager& resourceManager);

private:

    static std::unique_ptr<Animation> parseAnimation(tinyxml2::XMLElement* xml,
        const AnimatedGraphics* animated,
        std::unique_ptr<ValueCalculator> calculator,
        ResourceManager& resourceManager);
};

#endif // LEVEL_FILE_LOADER_HPP
