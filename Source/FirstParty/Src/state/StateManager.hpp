#pragma once

#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"

#include "SFML/Graphics/RenderWindow.hpp"

#include <memory> // unique_ptr
#include <vector>
#include <map>
#include <string>

/// This class is used to manage the diffrent states
class StateManager
{
public:

    StateManager();

    void registerState(StateId id, std::unique_ptr<State> state);
    void setState(StateId id, EnterStateInformation* enterInformation = nullptr);

    void draw(const DrawParameter& parameter);
    void update();
    void passEvent(utility::Event::EventType type);

private:
    State* getState(StateId id) const;

    sf::Clock m_frametime;
    double m_currentTime;
    std::map<StateId, std::unique_ptr<State>> m_statesById;
    State* m_currentState;
    StateId m_currentStateId;
    bool m_paused;
};

#endif // STATE_MANAGER_HPP
