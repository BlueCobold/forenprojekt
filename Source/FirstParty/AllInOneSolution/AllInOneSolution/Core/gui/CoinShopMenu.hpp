#pragma once

#ifndef COIN_SHOP_MENU_HPP
#define COIN_SHOP_MENU_HPP

#include "CaptionMenu.hpp"
#include "../model/Goody.hpp"

class CoinShopMenu : public CaptionMenu
{
public:
    static const int BUTTON_CLOSE = 17;
    static const int SPRITE_GRAVITY = 1;
    static const int LABEL_GRAVITY = 2;
    static const int SPRITE_INVULNERABLE = 5;
    static const int LABEL_INVULNERABLE = 6;
    static const int SPRITE_EXTRA_BALL = 9;
    static const int LABEL_EXTRA_BALL = 10;
    static const int SPRITE_EXTRA_TIME = 13;
    static const int LABEL_EXTRA_TIME = 14;
    
    CoinShopMenu(const sf::Vector2f& position, sf::RenderWindow& screen, ResourceManager& resourceManager);

    void setGoodyCharges(const Goody::Type& goody, const int charges);

private:

    static const int Deselected = 0;
    static const int Disabled = 3;

    void setGoodyIcon(const Goody::Type& goody, const int charges);
};

#endif