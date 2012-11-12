#pragma once

#include "State.hpp"

#include <vector>

/// This class is used to manage the diffrent states
class StateManager
{
public:

    StateManager();
    virtual ~StateManager();

    void push(std::unique_ptr<State> state);

    void next();
    void prev();

    void draw();
    void update();

private:

    std::vector<std::unique_ptr<State>> m_states;
    unsigned int m_index;

};

