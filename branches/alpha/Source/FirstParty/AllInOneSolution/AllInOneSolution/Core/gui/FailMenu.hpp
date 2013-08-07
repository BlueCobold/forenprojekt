#pragma once

#ifndef FAIL_MENU_HPP
#define FAIL_MENU_HPP

#include "CaptionMenu.hpp"

class FailMenu : public CaptionMenu
{
public:

    static const int BUTTON_PLAY_AGAIN = 0;
    static const int BUTTON_MAIN_MENU = 1;

    FailMenu(const sf::Vector2f& position,
             sf::RenderWindow& screen,
             ResourceManager& resourceManager);
};

#endif // FAIL_MENU_HPP