#pragma once

#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "Menu.hpp"

class MainMenu : public Menu
{
public:

    static const int BUTTON_START_NEW_GAME = 0;
    static const int BUTTON_SELECT_LEVEL = 1;
    static const int BUTTON_OPTIONS = 2;
    static const int BUTTON_CREDITS = 3;
    static const int BUTTON_CLOSE = 4;
    static const int BUTTON_ACHIEVEMENT = 6;
    
    static const int BUTTON_GERMAN_SHOW = 7;
    static const int BUTTON_ENGLISH_SHOW = 8;

    MainMenu(sf::RenderWindow& screen, ResourceManager& resourceManager);
};

#endif // MAIN_MENU_HPP
