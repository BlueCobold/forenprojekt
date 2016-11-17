#pragma once

#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"
#include "../gui/hud/HUD.hpp"
#include "../gui/PauseMenu.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

#include <memory>

/// This is the active game state when the game is paused.
class PauseState : public State
{
public:

    PauseState(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config);
    ~PauseState();

    StateChangeInformation update(const double time) override;
    void onEnter(const EnterStateInformation* enterInformation, const double time) override;

private:
    void renderStateTexture();
    void render(sf::RenderTarget& m_renderTexture);
    void doDraw(const DrawParameter& params) override;

    std::unique_ptr<sf::Texture> m_background;
    PauseMenu m_menu;
    HUD m_HUD;

    sf::RenderTexture m_renderTexture;
    Level* m_level;
    int m_levelNumber;

    EnterPlayStateInformation m_playStateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
    EnterOptionStateInformation m_optionStateInfo;
    EnterCoinShopStateInformation m_coinShopStateInfo;
    EnterLoadLevelStateInformation m_loadLevelStateInfo;
};

#endif // PAUSESTATE_HPP
