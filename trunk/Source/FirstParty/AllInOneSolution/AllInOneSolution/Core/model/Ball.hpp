#pragma once

#ifndef BALL_HPP
#define BALL_HPP

#include "Entity.hpp"

/// This class will be used to bind the physical setting and the control to a entity of a ball
class Ball : public Entity
{
public:

    Ball(float resetTime, const Entity* spawnAnimationEntity = nullptr);

    virtual void update(const float value);

    void setFieldDimension(const b2Vec2 fieldDimension);

    bool getBallLost();

    const Entity* getSpawnAnimationEntity() const;

private:

    float m_ballResetTime;
    float m_resetTime;
    float m_hideTime;

    b2Vec2 m_fieldDimension;

    bool m_lostBall;
    const Entity* m_spawnAnimationEntity;

private:

    void autoResetBall(const float elapsedTime);

    bool isOutside();

};

#endif