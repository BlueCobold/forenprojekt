#pragma once

#ifndef BALL_HPP
#define BALL_HPP

#include "Entity.hpp"

/// Only for Ball
class Ball : public Entity
{
private:

    float m_ballResetTime;
    b2Vec2 m_startPosition;

public:

    Ball(b2Vec2 startPosition);

    void update(const float elapsedTime);

    void autoResetBall(const float elapsedTime, float width, float height);
};

#endif