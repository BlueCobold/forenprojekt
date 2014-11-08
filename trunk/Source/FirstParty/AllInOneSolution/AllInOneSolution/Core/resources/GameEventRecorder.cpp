#include "GameEventRecorder.hpp"
#include "../Utility.hpp"

GameEventRecorder::GameEventRecorder() :
    m_lastGameEventType(GameEvent::Unknown)
{
}

void GameEventRecorder::addEvent(const float passedTime, const float ballSpeed, const GameEvent::GameEventType type)
{
    if(type != GameEvent::HitTeeter || m_lastGameEventType != GameEvent::HitTeeter)
    {
        m_gameEventList.push_back(GameEvent(passedTime, ballSpeed, type));
        m_lastGameEventType = type;
    }
}

void GameEventRecorder::clearList()
{
    m_gameEventList.clear();
}

const std::string GameEventRecorder::getEventsForOnlineHighscore() const
{
    std::string result;
    std::string value;
    unsigned int counterTeeter = 0;
    unsigned int counterTarget = 0;
    unsigned int counterBonusTarget = 0;
    unsigned int counterLostBall = 0;
    unsigned int counterGravity = 0;
    unsigned int counterTargetSpeeding = 0;
    unsigned int counterBonusTargetSpeeding = 0;

    for(auto it = std::begin(m_gameEventList); it != std::end(m_gameEventList); ++it)
    {
        // get time as millisecond
        value = utility::toString(static_cast<unsigned int>(it->getPassedTime() * 1000));

        if(it->getEventType() == GameEvent::HitTeeter)
            result.append("&t[" + utility::toString(counterTeeter++) + "]=" + value);
        else if(it->getEventType() == GameEvent::HitTarget)
            result.append("&m[" + utility::toString(counterTarget++) + "]=" + value);
        else if(it->getEventType() == GameEvent::HitBonusTarget)
            result.append("&b[" + utility::toString(counterBonusTarget++) + "]=" + value);
        else if(it->getEventType() == GameEvent::LostBall)
            result.append("&l[" + utility::toString(counterLostBall++) + "]=" + value);
        else if(it->getEventType() == GameEvent::GravityGoody)
            result.append("&g[" + utility::toString(counterGravity++) + "]=" + value);
        else if(it->getEventType() == GameEvent::HitTargetSpeeding)
            result.append("&ms[" + utility::toString(counterTargetSpeeding++) + "]=" + value);
        else if(it->getEventType() == GameEvent::HitBonusTargetSpeeding)
            result.append("&bs[" + utility::toString(counterBonusTargetSpeeding++) + "]=" + value);
        else
            throw std::runtime_error(utility::replace(utility::translateKey("UnknownGameEvent"), utility::toString(it->getEventType())));
    }

    return result;
}