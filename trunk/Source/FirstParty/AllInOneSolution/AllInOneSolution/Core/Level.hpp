#pragma once

#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "Entity.hpp"

#include <Box2D/Dynamics/b2World.h>

#include <vector>

class ResourceManager;

/// Manages the different levels and is
/// also resposible for loading them.
class Level
{
public:
    Level(const unsigned int level, ResourceManager& resourceManager);
    ~Level();

    /// Return the level number
    const unsigned int number();

private:
    /// Load the level after m_number
    bool load();

    /// Construct the full level filename from the level number
    const std::string filename();

private:
    ResourceManager& m_resourceManager;

    unsigned int m_number;

    std::vector<Entity> m_entities;

    b2World m_world;
};

#endif // LEVEL_HPP
