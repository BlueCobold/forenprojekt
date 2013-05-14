#pragma once

#ifndef LEVELFAILSTATE_HPP
#define LEVELFAILSTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"
#include "gui/Button.hpp"
#include "gui/hud/HUD.hpp"
#include "gui/FailMenu.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

#include <memory>

/// This is the active game state when the level is done.
class LevelFailState : public State
{
public:

    LevelFailState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config);
    ~LevelFailState();

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
    FailMenu m_menu;
    bool m_replay;

    EnterPlayStateInformation m_playStateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
    float m_timeDiff;
};

#endif // LEVELFAILSTATE_HPP
