#pragma once

#ifndef LEVEL_FILE_LOADER_HPP
#define LEVEL_FILE_LOADER_HPP

#include "../animation/Animation.hpp"
#include "../animation/ParticleTrail.hpp"
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
        AnimatedGraphics* animated,
        VariableHandler* handler,
        ResourceManager& resourceManager,
        std::unordered_map<std::string, tinyxml2::XMLElement*>* functions);

    static void parseColorController(
        Animation* animation,
        tinyxml2::XMLElement* xml,
        AnimatedGraphics* animated,
        VariableHandler* handler,
        std::unordered_map<std::string, tinyxml2::XMLElement*>* functions);
    
    static void parsePositionController(
        Animation* animation,
        tinyxml2::XMLElement* xml,
        AnimatedGraphics* animated,
        VariableHandler* handler,
        std::unordered_map<std::string, tinyxml2::XMLElement*>* functions);
    
    static void parseScaleController(
        Animation* animation,
        tinyxml2::XMLElement* xml,
        AnimatedGraphics* animated,
        VariableHandler* handler,
        std::unordered_map<std::string, tinyxml2::XMLElement*>* functions);
    
    static void parseRotationController(
        Animation* animation,
        tinyxml2::XMLElement* xml,
        AnimatedGraphics* animated,
        VariableHandler* handler,
        std::unordered_map<std::string, tinyxml2::XMLElement*>* functions);

    static std::unordered_map<std::string, tinyxml2::XMLElement*> parseList(
        tinyxml2::XMLElement* xml, const std::string& name, const std::string& key);

    static std::vector<std::string> parseGrid(tinyxml2::XMLElement* xml);

    static void parseBodyDef(tinyxml2::XMLElement* element,
        Entity* entity,
        VariableHandler* handler,
        std::unordered_map<std::string, tinyxml2::XMLElement*>* functions,
        b2BodyDef& bodyDef,
        const sf::Vector2u& position);

    static void parseKinematics(tinyxml2::XMLElement* element,
        Entity* entity,
        VariableHandler* handler,
        std::unordered_map<std::string, tinyxml2::XMLElement*>* functions);

    static std::unique_ptr<ValueProvider> parseProvider(
        tinyxml2::XMLElement* xml, 
        AnimatedObject* animated,
        VariableHandler* handler,
        Stoppable* stoppable,
        std::unordered_map<std::string, tinyxml2::XMLElement*>* functions);

    static void parseConstants(tinyxml2::XMLElement* xml,
        VariableHandler* holder);

    static std::unique_ptr<ParticleTrail> parseTrail(
        AnimatedGraphics* animated,
        tinyxml2::XMLElement* xml,
        ResourceManager& resourceManager,
        std::unordered_map<std::string, tinyxml2::XMLElement*>* functions);

private:

    static std::vector<std::unique_ptr<ValueProvider>> parseProviders(
        tinyxml2::XMLElement* xml, 
        AnimatedObject* animated,
        VariableHandler* handler,
        Stoppable* stoppable,
        std::unordered_map<std::string, tinyxml2::XMLElement*>* functions);

    static std::unique_ptr<ValueProvider> findController(tinyxml2::XMLElement* xml,
        AnimatedGraphics* animated,
        VariableHandler* handler,
        Stoppable* stoppable,
        const std::string& childName,
        const std::string& propertyName,
        const std::string& propertyValue,
        std::unordered_map<std::string, tinyxml2::XMLElement*>* functions);
};

#endif // LEVEL_FILE_LOADER_HPP
