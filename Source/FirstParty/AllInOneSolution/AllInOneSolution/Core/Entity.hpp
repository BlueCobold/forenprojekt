#pragma once

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "PhysicalObject.hpp"
#include "GraphicalObject.hpp"
#include "Animation.hpp"

/// This class will be used to draw objects that have a binding
/// to Box2D 
class Entity : public PhysicalObject, public GraphicalObject
{
public:

    Entity();
    virtual ~Entity();

    virtual void update();
    virtual void update( const float value );

    // This functions only exist because the physic system is not yet implemented
    // -------------------------------------
    void setPosition(float x, float y);
    
    void setRotation(float angle);

    sf::Vector2<float> getPosition();
    float getRotation();

    // -------------------------------------

    void bindAnimation(const bool infinite, const float min, const float step,
        const unsigned int numFrames, const unsigned int frameWidth, const unsigned int frameHeight);

    Animation& getAnimation();

protected:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:

    Animation m_animation;
    bool m_animated;

};

#endif // ENTITY_HPP
