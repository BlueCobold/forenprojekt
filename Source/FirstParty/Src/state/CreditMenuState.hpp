#pragma once

#ifndef CREDITMENUSTATE_HPP
#define CREDITMENUSTATE_HPP

#include "State.hpp"
#include "../gui/CreditMenu.hpp"

/// This is the option menu.
class CreditMenuState : public State
{
public:

    CreditMenuState(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config);
    ~CreditMenuState();

    StateChangeInformation update(const double time) override;
    void onEnter(const EnterStateInformation* enterInformation, const double time) override;

private:
    CreditMenu m_menu;

    EnterStateInformation m_stateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;

    void doDraw(const DrawParameter& params) override;
};

#endif // CREDITMENUSTATE_HPP