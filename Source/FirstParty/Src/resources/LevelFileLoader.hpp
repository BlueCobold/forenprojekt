#pragma once

#ifndef LEVEL_FILE_LOADER_HPP
#define LEVEL_FILE_LOADER_HPP

#include "../animation/Animation.hpp"
#include "../animation/CloneHandler.hpp"
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
        const tinyxml2::XMLElement* xml,
        AnimatedGraphics* animated,
        VariableHandler* handler,
        ResourceManager& resourceManager,
        std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
        CloneHandler& cloneHandler);

    static void parseColorController(
        Animation* animation,
        const tinyxml2::XMLElement* xml,
        AnimatedGraphics* animated,
        VariableHandler* handler,
        std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
        CloneHandler& cloneHandler);
    
    static void parsePositionController(
        Animation* animation,
        const tinyxml2::XMLElement* xml,
        AnimatedGraphics* animated,
        VariableHandler* handler,
        std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
        CloneHandler& cloneHandler);
    
    static void parseScaleController(
        Animation* animation,
        const tinyxml2::XMLElement* xml,
        AnimatedGraphics* animated,
        VariableHandler* handler,
        std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
        CloneHandler& cloneHandler);
    
    static void parseRotationController(
        Animation* animation,
        const tinyxml2::XMLElement* xml,
        AnimatedGraphics* animated,
        VariableHandler* handler,
        std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
        CloneHandler& cloneHandler);

    static std::unordered_map<std::string, const tinyxml2::XMLElement*> parseList(
        const tinyxml2::XMLElement* xml,
        const std::string& name,
        const std::string& key);

    static std::vector<std::string> parseGrid(const tinyxml2::XMLElement* xml);

    static void parseBodyDef(const tinyxml2::XMLElement* element,
        Entity* entity,
        VariableHandler* handler,
        std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
        b2BodyDef& bodyDef,
        const sf::Vector2u& position,
        CloneHandler& cloneHandler);

    static void parseKinematics(const tinyxml2::XMLElement* element,
        Entity* entity,
        VariableHandler* handler,
        std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
        CloneHandler& cloneHandler);

    static std::unique_ptr<ValueProvider> parseProvider(
        const tinyxml2::XMLElement* xml, 
        AnimatedObject* animated,
        VariableHandler* handler,
        Stoppable* stoppable,
        std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
        CloneHandler& cloneHandler);

    static void parseConstants(const tinyxml2::XMLElement* xml,
        VariableHandler* holder);

    static std::unique_ptr<ParticleTrail> parseTrail(
        AnimatedGraphics* animated,
        const tinyxml2::XMLElement* xml,
        ResourceManager& resourceManager,
        std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
        CloneHandler& cloneHandler);

private:

    static std::vector<std::unique_ptr<ValueProvider>> parseProviders(
        const tinyxml2::XMLElement* xml, 
        AnimatedObject* animated,
        VariableHandler* handler,
        Stoppable* stoppable,
        std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
        CloneHandler& cloneHandler);

    static std::unique_ptr<ValueProvider> findController(const tinyxml2::XMLElement* xml,
        AnimatedGraphics* animated,
        VariableHandler* handler,
        Stoppable* stoppable,
        const std::string& childName,
        const std::string& propertyName,
        const std::string& propertyValue,
        std::unordered_map<std::string, const tinyxml2::XMLElement*>* functions,
        CloneHandler& cloneHandler);
};

#endif // LEVEL_FILE_LOADER_HPP
