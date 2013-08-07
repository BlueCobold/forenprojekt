#pragma once

#ifndef OPTIONMENUSTATE_HPP
#define OPTIONMENUSTATE_HPP

#include "State.hpp"
#include "../gui/OptionMenu.hpp"
#include "../gui/hud/HUD.hpp"

class Level;

/// This is the option menu.
class OptionMenuState : public State
{
public:

    OptionMenuState(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config);
    ~OptionMenuState();

    virtual StateChangeInformation update(const float time);
    virtual void draw(const DrawParameter& params);
    virtual void onEnter(const EnterStateInformation* enterInformation, const float time);

private:
    OptionMenu m_menu;
    Level* m_level;
    HUD m_HUD;

    EnterPauseStateInformation m_pauseStateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
};

#endif // OPTIONMENUSTATE_HPP