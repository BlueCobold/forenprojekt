#pragma once
#ifndef BALL_HPP
#define BALL_HPP

#include "Entity.hpp"


///
class Ball : public Entity
{
public:

    Ball(b2World& world);
    ~Ball(void);

private:
    
    b2Body* ballBody;
    //b2PolygonShape ballBox;
    b2BodyDef ballDef;
};

#endif /// BALL_HPP