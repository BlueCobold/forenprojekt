#pragma once

#ifndef TRANSITIONSTATE_HPP
#define TRANSITIONSTATE_HPP

#include "State.hpp"
#include "rendering/transitions/Transition.hpp"

#include <memory>

/// This state is responsible for showing a transition.
class TransitionState : public State
{
public:

    TransitionState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config);
    ~TransitionState();

    StateChangeInformation update();
    void draw();
	void onEnter(void *enterInformation);

private:

    std::unique_ptr<Transition> m_transition;
    StateId m_followingState;
    void* m_followingEnterInformation;

};

#endif // TRANSITIONSTATE_HPP