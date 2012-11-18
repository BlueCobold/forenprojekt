#pragma once
#ifndef BALL_HPP
#define BALL_HPP

#include "Entity.hpp"


///
class Ball : public GraphicalObject
{
public:

    Ball(b2World& world, float radius, float density, float friction);
    
    ~Ball(void);

    void update();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    /// If Ball killed, you can give him a new position
    void setNewPosition(float x, float y);
    void setNewPosition(b2Vec2 position);

private:
    
    b2Body* m_ballBody;
    b2CircleShape m_ballShape;
    b2BodyDef m_ballDef;
    b2FixtureDef m_ballFixtureDef;
};

#endif /// BALL_HPP