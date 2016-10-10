#pragma once

#ifndef STARTSTATE_HPP
#define STARTSTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"

class StartState : public State
{
public:

    StartState(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config);
    ~StartState();

    StateChangeInformation update(const float time) override;
    void onEnter(const EnterStateInformation* enterInformation, const float time) override;

private:
    EnterTransitionStateInformation m_transitionStateInfo;
    EnterStateInformation m_stateInfo;

    void doDraw(const DrawParameter& params) override;
};

#endif // STARTSTATE_HPP