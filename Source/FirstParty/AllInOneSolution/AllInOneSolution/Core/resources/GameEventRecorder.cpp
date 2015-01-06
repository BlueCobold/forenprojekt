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

const std::list<GameEvent>& GameEventRecorder::getEventsForOnlineHighscore() const
{
    return m_gameEventList;
}