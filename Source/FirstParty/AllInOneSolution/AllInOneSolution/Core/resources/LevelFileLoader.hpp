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
     static std::unique_ptr<Animation> parseAnimation(
         tinyxml2::XMLElement* xml,
         const AnimatedGraphics* animated,
         ResourceManager& resourceManager);

private:

    static std::unique_ptr<ValueProvider> parseProvider(
        tinyxml2::XMLElement* xml, 
        const AnimatedGraphics* animated);

    static std::unique_ptr<Animation> parseAnimation(tinyxml2::XMLElement* xml,
        const AnimatedGraphics* animated,
        std::unique_ptr<ValueProvider> provider,
        ResourceManager& resourceManager);
};

#endif // LEVEL_FILE_LOADER_HPP
