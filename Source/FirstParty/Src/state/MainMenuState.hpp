#pragma once

#ifndef MAINMENUSTATE_HPP
#define MAINMENUSTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"
#include "../gui/Button.hpp"
#include "../gui/MainMenu.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

#include <memory>

/// This is the main menu.
class MainMenuState : public State
{
public:

    MainMenuState(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config);
    ~MainMenuState();

    StateChangeInformation update(const float time) override;
    void onEnter(const EnterStateInformation* enterInformation, const float time) override;

    void setLanguage(const std::string& language) override;

private:
    void renderStateTexture();
    void render(sf::RenderTarget& m_renderTexture);
    void doDraw(const DrawParameter& params) override;

    MainMenu m_menu;

    EnterStateInformation m_stateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
    EnterOptionStateInformation m_optionStateInfo;
    EnterLoadLevelStateInformation m_loadLevelStateInfo;
};

#endif // MAINMENUSTATE_HPP