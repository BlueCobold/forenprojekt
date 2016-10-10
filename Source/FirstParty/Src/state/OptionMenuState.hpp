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

    StateChangeInformation update(const float time) override;
    void onEnter(const EnterStateInformation* enterInformation, const float time) override;

    void setLanguage(const std::string& language) override;

private:
    void doDraw(const DrawParameter& params) override;

    OptionMenu m_menu;
    HUD m_HUD;
    Level* m_level;

    int m_clicked;

    EnterPauseStateInformation m_pauseStateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
};

#endif // OPTIONMENUSTATE_HPP
