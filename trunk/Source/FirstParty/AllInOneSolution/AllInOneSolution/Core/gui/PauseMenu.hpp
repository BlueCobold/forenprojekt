#pragma once

#ifndef PAUSE_MENU_HPP
#define PAUSE_MENU_HPP

#include "CaptionMenu.hpp"

class PauseMenu : public CaptionMenu
{
public:

    static const int BUTTON_CONTINUE = 0;
    static const int BUTTON_OPTIONS = 1;
    static const int BUTTON_RESTART_LEVEL = 2;
    static const int BUTTON_MAIN_MENU = 3;

    PauseMenu(const sf::Vector2f& position, sf::RenderWindow& screen, ResourceManager& resourceManager);
};

#endif // PAUSE_MENU_HPP
