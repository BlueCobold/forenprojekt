#pragma once

#ifndef LEVEL_PREVIEW_STATE_HPP
#define LEVEL_PREVIEW_STATE_HPP

#include "State.hpp"
#include "../gui/LevelPreviewMenu.hpp"
#include "../gui/hud/HUD.hpp"
#include "../model/Level.hpp"
#include "../resources/BackgroundLoader.hpp"

#include <functional>
#include <memory>

class Level;

/// This is the option menu.
class LevelPreviewState : public State
{
public:
    typedef std::function<void(std::unique_ptr<Level>&)> LoadCallback; // & should not exist, VS2010 bug requires it
    LevelPreviewState(sf::RenderWindow& screen,
                      ResourceManager& resourceManager,
                      AppConfig& config,
                      LoadCallback onLevelLoaded);
    ~LevelPreviewState();

    StateChangeInformation update(const double time) override;
    void onEnter(const EnterStateInformation* enterInformation, const double time) override;

private:
    void doDraw(const DrawParameter& params) override;

    LevelPreviewMenu m_menu;
    HUD m_HUD;
    Level* m_level;
    std::unique_ptr<Level> m_loadedLevel;
    bool m_levelUpdated;
    LoadCallback m_onLevelLoaded;
    std::string m_loadingErrorMessage;

    EnterPlayStateInformation m_playStateInfo;
    EnterHighScoreStateInformation m_highScoreInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
    EnterOptionStateInformation m_optionStateInfo;
    EnterCoinShopStateInformation m_coinShopStateInfo;
    
    std::map<int, LevelInfo> m_levelInfos;
    int m_nextLevelNumber;
    int m_unlockedLevel;
    std::unique_ptr<BackgroundLoader<LevelPreviewState>> m_levelLoaderJob;

    void loadLevelInfos();
    void updateButton(int id, int levelNumber);
    void loadLevel();
};

#endif // LEVEL_PREVIEW_STATE_HPP
