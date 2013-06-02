#pragma once

#ifndef STARTSTATE_HPP
#define STARTSTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"

class StartState : public State
{
public:

    StartState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config);
    ~StartState();

    virtual StateChangeInformation update(const float time);
    virtual void draw(const DrawParameter& params);
    virtual void onEnter(const EnterStateInformation* enterInformation, const float time);

private:
    EnterTransitionStateInformation m_transitionStateInfo;
    EnterPlayStateInformation m_playStateInfo;
};

#endif // STARTSTATE_HPP