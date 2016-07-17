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

    void setLanguage(const std::string& language) override;
private:
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
