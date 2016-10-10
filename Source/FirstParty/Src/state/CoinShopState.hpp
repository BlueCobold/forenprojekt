#pragma once

#ifndef COINSHOPSTATE_HPP
#define COINSHOPSTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"
#include "../gui/CoinShopMenu.hpp"
#include "../gui/hud/HUD.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

#include <memory>

class AchievementManager;

class CoinShopState : public State
{
public:

    CoinShopState(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config, AchievementManager& achievementManager);
    ~CoinShopState();

    StateChangeInformation update(const float time) override;
    void onEnter(const EnterStateInformation* enterInformation, const float time) override;

    void setLanguage(const std::string& language) override;

private:
    CoinShopMenu m_menu;
    Level* m_level;
    HUD m_HUD;
    int m_levelNumber;
    int m_clicked;

    EnterTransitionStateInformation m_transitionStateInfo;
    EnterPlayStateInformation m_levelPreviewInfo;
    EnterPauseStateInformation m_pauseStateInfo;

    AchievementManager& m_achievementManager;

    void doDraw(const DrawParameter& params) override;

    void updateButtons();
    void buy(const Goody::Type type, const std::string& propertyName);
    void sell(const Goody::Type type, const std::string& propertyName);
};

#endif