#pragma once

#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include "State.hpp"
#include "model/Level.hpp"
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

    StateManager(sf::RenderWindow& screen);

	void registerState(StateId id, std::shared_ptr<State> state);
    void setState(StateId id, EnterStateInformation* enterInformation = nullptr);

    void draw();
    void update();
    void passEvent(utility::Event::EventType type);

private:
    State* getState(StateId id) const;

    sf::RenderWindow& m_screen;
    sf::Clock m_frametime;
    float m_currentTime;
	std::map<StateId, std::shared_ptr<State>> m_statesById;
    State* m_currentState;
    StateId m_currentStateId;
    std::unique_ptr<Level> m_currentLevel;
};

#endif // STATE_MANAGER_HPP
