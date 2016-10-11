#pragma once

#ifndef BALL_HPP
#define BALL_HPP

#include "Entity.hpp"

#include <functional>
#include "../animation/ParticleTrail.hpp"

/// This class will be used to bind the physical setting and the control to a entity of a ball
class Ball : public Entity
{
public:

    Ball(float resetTime,
         float stuckBallSpeed,
         CloneHandler& cloneHandler,
         const Entity* spawnAnimationEntity = nullptr);

    void update(const float value) override;

    float getValueOf(const std::string& name) const override;
    void setValueOf(const std::string& name, const float value) override;

    void registerForCheckpointChanges(std::function<void()> handler);

    void setFieldDimension(const b2Vec2 fieldDimension);
    void setInvulnerable(const bool value);
    void checkpointReached(const b2Vec2 position);
    bool getBallLost() const;

    const Entity* getSpawnAnimationEntity() const;

    void blowUp();
    bool blownUp() const;
    bool isSpeeding() const;
    void teleportTo(const float x, const float y, const bool resetSpeed);

    void bindTrail(std::unique_ptr<ParticleTrail> trail);

private:

    float m_ballResetTime;
    float m_resetTime;
    float m_hideTime;
    float m_blowUpTime;

    b2Vec2 m_spawnPosition;
    b2Vec2 m_fieldDimension;

    bool m_lostBall;
    bool m_blownUp;
    bool m_isTeleporting;
    bool m_resetSpeedOnTeleport;
    b2Vec2 m_teleportPosition;
    const Entity* m_spawnAnimationEntity;
    std::function<void()> m_checkpointHandler;
    std::unique_ptr<ParticleTrail> m_trail;

    float m_stuckBallSpeed;
    float m_stuckBallTime;
    bool m_isStucking;

    void autoResetBall(const float elapsedTime);
    void doDraw(const DrawParameter& params) override;
    void onRestarted() override;

    bool isOutside();

};

#endif