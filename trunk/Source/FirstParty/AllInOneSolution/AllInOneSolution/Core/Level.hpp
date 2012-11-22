#pragma once

#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "Entity.hpp"

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Collision/Shapes/b2Shape.h>

#include <vector>

class ResourceManager;

namespace sf
{
    class RenderWindow;
}

/// Manages the different levels and is
/// also resposible for loading them.
class Level
{
public:
    Level(const unsigned int level, ResourceManager& resourceManager);
    ~Level();

    void update(const float dt);
    void draw(sf::RenderWindow& screen);

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

    // HACK: This should be in the Entity class, but since unique_ptr sucks with VS10...
    std::vector<std::unique_ptr<b2Shape>> m_shapes;

    b2World m_world;
    float m_timeStep;
    int m_velocityIterations;
    int m_positionIterations;
};

#endif // LEVEL_HPP
