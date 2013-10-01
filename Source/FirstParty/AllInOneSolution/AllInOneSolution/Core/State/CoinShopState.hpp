#pragma once

#ifndef COINSHOPSTATE_HPP
#define COINSHOPSTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"
#include "../gui/CoinShopMenu.hpp"
#include "../gui/hud/HUD.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

#include <memory>

class CoinShopState : public State
{
public:

    CoinShopState(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config);
    ~CoinShopState();

    virtual StateChangeInformation update(const float time);
    virtual void draw(const DrawParameter& params);
    virtual void onEnter(const EnterStateInformation* enterInformation, const float time);

private:
    CoinShopMenu m_coinShopMenu;
    Level* m_level;
    HUD m_HUD;
    int m_levelNumber;
    int m_clicked;

    EnterTransitionStateInformation m_transitionStateInfo;
    EnterPlayStateInformation m_levelPreviewInfo;

    void updateButtons();
    void buy(const Goody::Type type, const std::string& propertyName);
    void sell(const Goody::Type type, const std::string& propertyName);
};

#endif