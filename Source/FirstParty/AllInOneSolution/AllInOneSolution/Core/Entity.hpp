#pragma once

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "PhysicalObject.hpp"
#include "GraphicalObject.hpp"

/// This class will be used to draw objects that have a binding
/// to Box2D 
class Entity : public PhysicalObject, public GraphicalObject
{
public:

    Entity();
    virtual ~Entity();

    void update();

private:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

#endif // ENTITY_HPP
