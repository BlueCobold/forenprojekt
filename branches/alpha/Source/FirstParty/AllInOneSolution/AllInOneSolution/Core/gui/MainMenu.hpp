#pragma once

#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "CaptionMenu.hpp"

class MainMenu : public CaptionMenu
{
public:

    static const int BUTTON_START_NEW_GAME = 0;
    static const int BUTTON_SELECT_LEVEL = 1;
    static const int BUTTON_OPTIONS = 2;
    static const int BUTTON_CREDITS = 3;
    static const int BUTTON_CLOSE = 4;

    MainMenu(const sf::Vector2f& position, sf::RenderWindow& screen, ResourceManager& resourceManager);
};

#endif // MAIN_MENU_HPP