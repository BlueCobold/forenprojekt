#pragma once

#ifndef REPLAY_MENU_HPP
#define REPLAY_MENU_HPP

#include "CaptionMenu.hpp"

class ReplayMenu : public CaptionMenu
{
public:

    static const int BUTTON_PLAY_AGAIN = 0;
    static const int BUTTON_PLAY_NEXT = 1;
    static const int BUTTON_MAIN_MENU = 2;
    static const int BUTTON_HIGHSCORE = 30;
    static const int SPRITE_SYMBOL_STAR_BRONCE_DARK = 25;
    static const int SPRITE_SYMBOL_STAR_SILVER_DARK = 26;
    static const int SPRITE_SYMBOL_STAR_GOLD_DARK = 27;
    static const int SPRITE_SYMBOL_STAR_BRONCE_FILLED = 35;
    static const int SPRITE_SYMBOL_STAR_SILVER_FILLED = 36;
    static const int SPRITE_SYMBOL_STAR_GOLD_FILLED = 37;
    static const int LABEL_POINTS = 10;
    static const int LABEL_LOSTBALLS = 12;

    ReplayMenu(sf::RenderWindow& screen,
               ResourceManager& resourceManager);

    void setPoints(int points);
    void setGrade(int grade);
    void setLostBalls(int lostBalls);
    void setMedalToolTipText(const std::string& text);

};

#endif // REPLAY_MENU_HPP
