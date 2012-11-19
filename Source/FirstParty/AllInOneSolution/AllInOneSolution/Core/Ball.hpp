#pragma once
#ifndef BALL_HPP
#define BALL_HPP

#include "Entity.hpp"


///
class Ball : public Entity
{
public:

    Ball(b2CircleShape ballShape);
    
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