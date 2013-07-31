#pragma once

#ifndef LEVELSELECTMENU_HPP
#define LEVELSELECTMENU_HPP

#include "CaptionMenu.hpp"

class LevelSelectMenu : public CaptionMenu
{
public:

    static const int BUTTON_SHOW_DETAILS = 0;
    static const int BUTTON_MAIN_MENU = 1;

    LevelSelectMenu(const sf::Vector2f& position, sf::RenderWindow& screen, ResourceManager& resourceManager);
};

#endif // LEVELSELECTMENU_HPP