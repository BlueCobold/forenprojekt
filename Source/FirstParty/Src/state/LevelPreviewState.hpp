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

    StateChangeInformation update(const double time) override;
    void onEnter(const EnterStateInformation* enterInformation, const double time) override;

private:
    void doDraw(const DrawParameter& params) override;

    LevelPreviewMenu m_menu;
    HUD m_HUD;
    Level* m_level;
    bool m_levelUpdated;
    int m_levelNumber;

    EnterPlayStateInformation m_playStateInfo;
    EnterHighScoreStateInformation m_highScoreInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
    EnterCoinShopStateInformation m_coinShopStateInfo;
};

#endif // LEVEL_PREVIEW_STATE_HPP
