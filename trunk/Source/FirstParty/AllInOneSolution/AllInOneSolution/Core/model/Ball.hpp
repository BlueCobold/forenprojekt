#pragma once

#ifndef BALL_HPP
#define BALL_HPP

#include "Entity.hpp"

/// This class will be used to bind the physical setting and the control to a entity of a ball
class Ball : public Entity
{
public:

    Ball(float resetTime);

    virtual void update(const float value);

    void setStartPosition(const b2Vec2 startPosition);

    void setFieldDimension(const b2Vec2 fieldDimension);

    bool getBallLost();

    bool getMultiHit();

    void setMultiHit(bool value);
private:

    float m_ballResetTime;
    float m_resetTime;

    b2Vec2 m_startPosition;
    b2Vec2 m_fieldDimension;

    bool m_lostBall;

    bool m_multiHit;
private:

    void autoResetBall(const float elapsedTime);

    bool isOutside();

};

#endif