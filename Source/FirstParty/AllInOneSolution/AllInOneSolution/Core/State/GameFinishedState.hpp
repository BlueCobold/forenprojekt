#pragma once

#ifndef GAMEFINISHEDSTATE_HPP
#define GAMEFINISHEDSTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"
#include "../gui/Button.hpp"
#include "../gui/GameFinishedMenu.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

#include <memory>

/// This is the game finished menu.
class GameFinishedState : public State
{
public:

    GameFinishedState(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config);
    ~GameFinishedState();

    virtual StateChangeInformation update(const float time);
    virtual void draw(const DrawParameter& params);
    virtual void onEnter(const EnterStateInformation* enterInformation, const float time);

private:
    void renderStateTexture();
    void render(sf::RenderTarget& m_renderTexture);

    GameFinishedMenu m_menu;

    EnterStateInformation m_stateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
};

#endif // GAMEFINISHEDSTATE_HPP