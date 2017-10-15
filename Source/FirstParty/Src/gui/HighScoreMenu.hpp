#pragma once

#ifndef HIGHSCOREMENU_HPP
#define HIGHSCOREMENU_HPP

#include "Menu.hpp"

class HighScoreMenu : public Menu
{
public:

    static const int BUTTON_CLOSE = 0;
    static const int LABEL_PLACES = 11;
    static const int LABEL_POINTS = 21;
    static const int CHECKBOX_LOCAL_HIGHSCORE = 30;
    static const int CHECKBOX_GAMEMODE = 31;
    static const int LABEL_LOADING = 27;
    static const int SUBWINDOW = 1;

    HighScoreMenu(sf::RenderWindow& screen, ResourceManager& resourceManager);
};

#endif // HIGHSCOREMENU_HPP