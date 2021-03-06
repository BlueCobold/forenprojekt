#pragma once

#ifndef GAMEEVENTRECORDER_HPP
#define GAMEEVENTRECORDER_HPP

#include "GameEvent.hpp"
#include <list>

class GameEventRecorder
{
private:
    std::list<GameEvent> m_gameEventList;
    GameEvent::GameEventType m_lastGameEventType;

public:
    GameEventRecorder();

    void addEvent(const double passedTime, const float ballSpeed, const GameEvent::GameEventType type);
    void clearList();

    const std::list<GameEvent>& getGameEvents() const;
};

#endif