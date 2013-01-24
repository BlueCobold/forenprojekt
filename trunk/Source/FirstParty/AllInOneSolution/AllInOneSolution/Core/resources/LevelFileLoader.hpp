#pragma once

#ifndef LEVEL_FILE_LOADER_HPP
#define LEVEL_FILE_LOADER_HPP

#include "../animation/Animation.hpp"
#include "../animation/provider/ValueProvider.hpp"
#include "../model/SoundObject.hpp"
#include "../model/AnimatedGraphics.hpp"
#include "../model/Entity.hpp"

#include "ResourceManager.hpp"

#include <memory> // unique_ptr, move
#include <string>
#include <unordered_map>
#include <vector>

#include <tinyxml2.h>

class LevelFileLoader
{
public:
     static std::unique_ptr<Animation> parseAnimation(
         tinyxml2::XMLElement* xml,
         const AnimatedGraphics* animated,
         ResourceManager& resourceManager);

     static std::unordered_map<std::string, tinyxml2::XMLElement*> parseList(
         tinyxml2::XMLElement* xml, const std::string& name, const std::string& key);

     static std::vector<std::string> parseGrid(tinyxml2::XMLElement* xml);

     static void parseKinematics(tinyxml2::XMLElement* element,
         Entity* entity);

      static sf::Sound* parseSound(tinyxml2::XMLElement* xml,
        ResourceManager& resourceManager);

private:

    static std::unique_ptr<ValueProvider> parseProvider(
        tinyxml2::XMLElement* xml, 
        const AnimatedGraphics* animated);
    
    static std::vector<std::unique_ptr<ValueProvider>> parseProviders(
        tinyxml2::XMLElement* xml, 
        const AnimatedGraphics* animated);

    static std::unique_ptr<Animation> parseAnimation(tinyxml2::XMLElement* xml,
        const AnimatedGraphics* animated,
        std::unique_ptr<ValueProvider> provider,
        ResourceManager& resourceManager);

    static std::unique_ptr<ValueProvider> findPositionController(tinyxml2::XMLElement* xml,
        const AnimatedGraphics* animated,
        const std::string& axis);

    static void parseConstants(tinyxml2::XMLElement* xml,
        const AnimatedGraphics* owner,
        Animation* holder);
};

#endif // LEVEL_FILE_LOADER_HPP
