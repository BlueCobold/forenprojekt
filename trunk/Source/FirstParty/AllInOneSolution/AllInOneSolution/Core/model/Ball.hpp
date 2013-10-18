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

    Ball(float resetTime, const Entity* spawnAnimationEntity = nullptr, const Entity* killAnimationEntity = nullptr);

    virtual void restartAt(const float value) override;
    virtual void update(const float value) override;
    virtual void draw(const DrawParameter& params) override;
    
    virtual float getValueOf(const std::string& name) const override;
    virtual void setValueOf(const std::string& name, const float value) override;

    void registerForCheckpointChanges(std::function<void()> handler);

    void setFieldDimension(const b2Vec2 fieldDimension);
    void setInvulnerable(const bool value);
    void checkpointReached(const b2Vec2 position);
    bool getBallLost() const;

    const Entity* getSpawnAnimationEntity() const;
    const Entity* getKillAnimationEntity() const;

    void blowUp();
    bool blownUp() const;

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
    const Entity* m_spawnAnimationEntity;
    const Entity* m_killAnimationEntity;
    std::function<void()> m_checkpointHandler;
    std::unique_ptr<ParticleTrail> m_trail;

private:

    void autoResetBall(const float elapsedTime);

    bool isOutside();

};

#endif