#pragma once

#ifndef OPTIONMENUSTATE_HPP
#define OPTIONMENUSTATE_HPP

#include "State.hpp"
#include "../gui/OptionMenu.hpp"
#include "../gui/hud/HUD.hpp"

class Level;

namespace sf
{
    class RenderWindow;
};

/// This is the option menu.
class OptionMenuState : public State
{
public:

    OptionMenuState(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config);
    ~OptionMenuState();

    StateChangeInformation update(const double time) override;
    void onEnter(const EnterStateInformation* enterInformation, const double time) override;

private:
    void doDraw(const DrawParameter& params) override;

    OptionMenu m_menu;
    HUD m_HUD;
    Level* m_level;

    int m_clicked;

    sf::RenderWindow& m_screen;
    EnterPauseStateInformation m_pauseStateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
};

#endif // OPTIONMENUSTATE_HPP
