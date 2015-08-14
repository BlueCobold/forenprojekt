#pragma once

#ifndef LEVEL_PREVIEW_STATE_HPP
#define LEVEL_PREVIEW_STATE_HPP

#include "State.hpp"
#include "../gui/LevelPreviewMenu.hpp"
#include "../gui/hud/HUD.hpp"

class Level;

/// This is the option menu.
class LevelPreviewState : public State
{
public:

    LevelPreviewState(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config);
    ~LevelPreviewState();

    virtual StateChangeInformation update(const float time);
    virtual void draw(const DrawParameter& params);
    virtual void onEnter(const EnterStateInformation* enterInformation, const float time);

private:
    LevelPreviewMenu m_menu;
    Level* m_level;
    bool m_levelUpdated;
    HUD m_HUD;
    int m_levelNumber;

    EnterPlayStateInformation m_playStateInfo;
    EnterHighScoreStateInformation m_highScoreInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
    EnterCoinShopStateInformation m_coinShopStateInfo;
};

#endif // LEVEL_PREVIEW_STATE_HPP