#pragma once

#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include "State.hpp"

#include <memory> // unique_ptr
#include <vector>
#include <map>
#include <string>

/// This class is used to manage the diffrent states
class StateManager
{
public:

    StateManager();
    ~StateManager();

	void registerState(StateId id, std::shared_ptr<State> state);
    void setState(StateId id, void* enterInformation= nullptr);

    void draw();
    void update();

private:

	std::map<StateId, std::shared_ptr<State>> m_statesById;
    std::shared_ptr<State> m_currentState;
};

#endif // STATE_MANAGER_HPP
