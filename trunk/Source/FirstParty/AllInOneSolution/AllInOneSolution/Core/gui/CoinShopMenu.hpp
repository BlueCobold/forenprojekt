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
    static const int BUTTON_GRAVITY_PLUS = 3;
    static const int BUTTON_GRAVITY_MINUS = 4;
    static const int SPRITE_INVULNERABLE = 5;
    static const int LABEL_INVULNERABLE = 6;
    static const int BUTTON_INVULNERABLE_PLUS = 7;
    static const int BUTTON_INVULNERABLE_MINUS = 8;
    static const int SPRITE_EXTRA_BALL = 9;
    static const int LABEL_EXTRA_BALL = 10;
    static const int BUTTON_EXTRA_BALL_PLUS = 11;
    static const int BUTTON_EXTRA_BALL_MINUS = 12;
    static const int SPRITE_EXTRA_TIME = 13;
    static const int LABEL_EXTRA_TIME = 14;
    static const int BUTTON_EXTRA_TIME_PLUS = 15;
    static const int BUTTON_EXTRA_TIME_MINUS = 16;
    
    CoinShopMenu(const sf::Vector2f& position, sf::RenderWindow& screen, ResourceManager& resourceManager);

    void setGoodyCharges(const Goody::Type& goody, const int charges);
    int getBuyCost(const Goody::Type& goody) const;
    int getSellRefund(const Goody::Type& goody) const;

private:

    // price
    static const int Price_Gravity = 1000;
    static const int Price_Invulnerable = 400;
    static const int Price_Extra_Ball = 400;
    static const int Price_Extra_Time = 500;
    static const float SellModifier;

    void setGoodyIcon(const Goody::Type& goody, const int charges);
    void setToolTip(int id, const std::string& textKey, int costs);
};

#endif