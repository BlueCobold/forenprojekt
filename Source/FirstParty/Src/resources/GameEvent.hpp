#pragma once

#ifndef GAMEEVENT_HPP
#define GAMEEVENT_HPP

class GameEvent
{
public:
    enum GameEventType {Unknown,
                        HitTarget,
                        HitTargetSpeeding,
                        HitBonusTarget,
                        HitBonusTargetSpeeding,
                        HitTeeter,
                        LostBall,
                        GravityGoody,
                        ExtraBallGoody,
                        ExtraTimeGoody,
                        InvulnerableGoody};
private:
    double m_passedTime;
    float m_ballSpeed;
    GameEventType m_type;

public:
    GameEvent(const double passedTime, const float ballSpeed, const GameEventType type) :
        m_passedTime(passedTime),
        m_ballSpeed(ballSpeed),
        m_type(type)
    {}

    double getPassedTime() const
    {
        return m_passedTime;
    }

    float getBallSpeed() const
    {
        return m_ballSpeed;
    }

    GameEventType getEventType() const
    {
        return m_type;
    }
};

#endif