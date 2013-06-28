#pragma once

#ifndef CREDITMENUSTATE_HPP
#define CREDITMENUSTATE_HPP

#include "State.hpp"
#include "../gui/CreditMenu.hpp"

/// This is the option menu.
class CreditMenuState : public State
{
public:

    CreditMenuState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config);
    ~CreditMenuState();

    virtual StateChangeInformation update(const float time);
    virtual void draw(const DrawParameter& params);
    virtual void onEnter(const EnterStateInformation* enterInformation, const float time);

private:
    CreditMenu m_menu;

    EnterStateInformation m_stateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
};

#endif // CREDITMENUSTATE_HPP