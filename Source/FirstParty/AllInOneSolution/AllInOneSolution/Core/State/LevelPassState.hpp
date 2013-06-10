#pragma once

#ifndef LEVELPASSSTATE_HPP
#define LEVELPASSSTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"
#include "../gui/Button.hpp"
#include "../gui/hud/HUD.hpp"
#include "../gui/ReplayMenu.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

#include <memory>

/// This is the active game state when the level is done.
class LevelPassState : public State
{
public:

    LevelPassState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config);
    ~LevelPassState();

    virtual StateChangeInformation update(const float time);
    virtual void draw(const DrawParameter& params);
    virtual void onEnter(const EnterStateInformation* enterInformation, const float time);

private:
    void renderStateTexture();
    void render(sf::RenderTarget& m_renderTexture);

    std::unique_ptr<sf::Texture> m_background;
    sf::RenderTexture m_renderTexture;
    Level* m_level;

    HUD m_HUD;
    ReplayMenu m_menu;
    bool m_replay;

    EnterPlayStateInformation m_playStateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
    float m_timeDiff;
};

#endif // LEVELPASSSTATE_HPP
