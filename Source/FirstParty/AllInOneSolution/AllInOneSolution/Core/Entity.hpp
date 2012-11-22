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

    virtual void update();

    // This functions only exist because the physic system is not yet implemented
    // -------------------------------------
    void setPosition(float x, float y);
    
    void setRotation(float angle);

    sf::Vector2<float> getPosition();
    float getRotation();

    // -------------------------------------

private:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

#endif // ENTITY_HPP
