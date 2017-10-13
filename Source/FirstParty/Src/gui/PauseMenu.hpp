#pragma once

#ifndef PAUSE_MENU_HPP
#define PAUSE_MENU_HPP

#include "Menu.hpp"

class PauseMenu : public Menu
{
public:

    static const int BUTTON_CONTINUE = 0;
    static const int BUTTON_OPTIONS = 1;
    static const int BUTTON_RESTART_LEVEL = 2;
    static const int BUTTON_MAIN_MENU = 3;
    static const int BUTTON_COINS = 4;
    static const int IMAGE_PREVIEW = 10;

    PauseMenu(sf::RenderWindow& screen, ResourceManager& resourceManager);

    void setCoinToolTipText(const std::string& text, const std::string& coins);
};

#endif // PAUSE_MENU_HPP
