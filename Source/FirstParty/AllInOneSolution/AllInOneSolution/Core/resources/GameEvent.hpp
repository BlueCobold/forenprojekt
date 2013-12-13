#pragma once

#ifndef GAMEEVENT_HPP
#define GAMEEVENT_HPP

class GameEvent
{
public:
    enum GameEventType {Unknown,
                        HitTarget,
                        HitBonusTarget,
                        HitTeeter};
private:
    float m_passedTime;
    float m_ballSpeed;
    GameEventType m_type;

public:
    GameEvent(const float passedTime, const float ballSpeed, const GameEventType type) :
        m_passedTime(passedTime),
        m_ballSpeed(ballSpeed),
        m_type(type)
    {}

    float getPassedTime() const
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