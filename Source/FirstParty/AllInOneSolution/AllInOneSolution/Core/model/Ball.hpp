#pragma once

#ifndef BALL_HPP
#define BALL_HPP

#include "Entity.hpp"

/// This class will be used to bind the physical setting and the control to a entity of a ball
class Ball : public Entity
{
public:

    Ball();

    virtual void update(const float value);

    void setStartPosition(b2Vec2 startPosition);

    void setFieldDimension(b2Vec2 fieldDimension);

private:

    float m_ballResetTime;

    b2Vec2 m_startPosition;
    b2Vec2 m_fieldDimension;

private:

    void autoResetBall(const float elapsedTime);
};

#endif