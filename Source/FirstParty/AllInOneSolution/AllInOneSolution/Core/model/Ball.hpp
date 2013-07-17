#pragma once

#ifndef BALL_HPP
#define BALL_HPP

#include "Entity.hpp"

#include <functional>

/// This class will be used to bind the physical setting and the control to a entity of a ball
class Ball : public Entity
{
public:

    Ball(float resetTime, const Entity* spawnAnimationEntity = nullptr, const Entity* killAnimationEntity = nullptr);

    virtual void restartAt(const float value) override;
    virtual void update(const float value) override;

    void registerForCheckpointChanges(std::function<void()> handler);

    void setFieldDimension(const b2Vec2 fieldDimension);
    void checkpointReached(const b2Vec2 position);
    bool getBallLost();

    const Entity* getSpawnAnimationEntity() const;
    const Entity* getKillAnimationEntity() const;

    void blowUp();
    bool blownUp();

private:

    float m_ballResetTime;
    float m_resetTime;
    float m_hideTime;
    float m_blowUpTime;

    b2Vec2 m_spawnPosition;
    b2Vec2 m_fieldDimension;

    bool m_lostBall;
    bool m_blownUp;
    const Entity* m_spawnAnimationEntity;
    const Entity* m_killAnimationEntity;
    std::function<void()> m_checkpointHandler;

private:

    void autoResetBall(const float elapsedTime);

    bool isOutside();

};

#endif