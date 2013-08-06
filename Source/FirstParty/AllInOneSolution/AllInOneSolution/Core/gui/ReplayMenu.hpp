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
    static const int SPRITE_SYMBOL_NONE = 14;
    static const int SPRITE_SYMBOL_BRONCE = 15;
    static const int SPRITE_SYMBOL_SILVER = 16;
    static const int SPRITE_SYMBOL_GOLD = 17;
    static const int LABEL_POINTS = 10;
    static const int LABEL_LOSTBALLS = 12;

    ReplayMenu(const sf::Vector2f& position,
               sf::RenderWindow& screen,
               ResourceManager& resourceManager);

    void setPoints(int points);
    void setGrade(int grade);
    void setLostBalls(int lostBalls);
    void setMedalToolTipText(const std::string& text);

};

#endif // REPLAY_MENU_HPP
