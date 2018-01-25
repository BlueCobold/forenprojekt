#pragma once

#ifndef REPLAY_MENU_HPP
#define REPLAY_MENU_HPP

#include "Menu.hpp"

class ReplayMenu : public Menu
{
public:

    static const int BUTTON_PLAY_AGAIN = 0;
    static const int BUTTON_PLAY_NEXT = 1;
    static const int BUTTON_MAIN_MENU = 2;
    static const int BUTTON_HIGHSCORE = 30;
    static const int SPRITE_SYMBOL_STAR_BRONCE_DARK = 25;
    static const int SPRITE_SYMBOL_STAR_SILVER_DARK = 26;
    static const int SPRITE_SYMBOL_STAR_GOLD_DARK = 27;
    static const int SPRITE_RIBBON_BRONCE = 14;
    static const int SPRITE_RIBBON_SILVER = 15;
    static const int SPRITE_RIBBON_GOLD = 16;
    static const int SPRITE_RIBBON_RED = 17;
    static const int SPRITE_SYMBOL_STAR_BRONCE_FILLED = 35;
    static const int SPRITE_SYMBOL_STAR_SILVER_FILLED = 36;
    static const int SPRITE_SYMBOL_STAR_GOLD_FILLED = 37;
    static const int LABEL_POINTS = 10;
    static const int LABEL_LOSTBALLS = 12;
    static const int LABEL_TIME = 41;

    ReplayMenu(sf::RenderWindow& screen,
               ResourceManager& resourceManager);

    void onEnter(double time);
    void setPoints(int points);
    void setGrade(int grade);
    void setLostBalls(int lostBalls);
    void setPlayTime(float playTime);
    void setMedalToolTipText(const std::string& text);
};

#endif // REPLAY_MENU_HPP
