#pragma once

#ifndef FAIL_MENU_HPP
#define FAIL_MENU_HPP

#include "Menu.hpp"

class FailMenu : public Menu
{
public:

    static const int BUTTON_PLAY_AGAIN = 0;
    static const int BUTTON_MAIN_MENU = 1;
    static const int BUTTON_HIGHSCORE = 2;

    FailMenu(sf::RenderWindow& screen,
             ResourceManager& resourceManager);
};

#endif // FAIL_MENU_HPP