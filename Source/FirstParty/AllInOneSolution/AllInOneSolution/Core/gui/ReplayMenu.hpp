#pragma once

#ifndef REPLAY_MENU_HPP
#define REPLAY_MENU_HPP

#include "CaptionMenu.hpp"

class ReplayMenu : public CaptionMenu
{
public:

    static const int BUTTON_PLAY_AGAIN = 0;
    static const int LABEL_POINTS = 1;
    static const int SPRITE_SYMBOL_NONE = 3;
    static const int SPRITE_SYMBOL_BRONCE = 4;
    static const int SPRITE_SYMBOL_SILVER = 5;
    static const int SPRITE_SYMBOL_GOLD = 6;
    static const int LABEL_LOSTBALLS = 7;
    static const int BUTTON_MAIN_MENU = 9;

    ReplayMenu(const sf::Vector2f& position,
               sf::RenderWindow& screen,
               ResourceManager& resourceManager);

    void setPoints(int points);
    void setGrade(int grade);
    void setLostBalls(int lostBalls);

};

#endif // REPLAY_MENU_HPP
