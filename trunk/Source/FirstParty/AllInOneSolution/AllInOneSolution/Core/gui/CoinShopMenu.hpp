#pragma once

#ifndef COIN_SHOP_MENU_HPP
#define COIN_SHOP_MENU_HPP

#include "CaptionMenu.hpp"

class CoinShopMenu : public CaptionMenu
{
public:
    static const int BUTTON_CLOSE = 0;
    
    CoinShopMenu(const sf::Vector2f& position, sf::RenderWindow& screen, ResourceManager& resourceManager);
};

#endif