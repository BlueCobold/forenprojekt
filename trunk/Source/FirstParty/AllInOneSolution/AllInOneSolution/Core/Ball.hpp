#pragma once
#ifndef BALL_HPP
#define BALL_HPP

#include "Entity.hpp"


/// a elementar game object
class Ball : public Entity
{
public:

    Ball(b2CircleShape ballShape, float xPos, float yPos, float radius, 
         float density, float friction, sf::Texture texture);
    
    ~Ball(void);

    void update();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    /// If Ball killed, you can give him a new position
    void setNewPosition(float x, float y);
    void setNewPosition(b2Vec2 position);

private:
    
    b2CircleShape m_ballShape;
};

#endif /// BALL_HPP