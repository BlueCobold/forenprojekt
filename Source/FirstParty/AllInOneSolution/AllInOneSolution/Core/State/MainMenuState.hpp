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

    MainMenuState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config);
    ~MainMenuState();

    virtual StateChangeInformation update(const float time);
    virtual void draw(const DrawParameter& params);
    virtual void onEnter(const EnterStateInformation* enterInformation, const float time);

private:
    void renderStateTexture();
    void render(sf::RenderTarget& m_renderTexture);

    MainMenu m_menu;

    EnterStateInformation m_stateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
    EnterOptionStateInformation m_optionStateInfo;
    float m_timeDiff;
};

#endif // MAINMENUSTATE_HPP