#pragma once

#ifndef LEVELSELECTMENU_HPP
#define LEVELSELECTMENU_HPP

#include "Menu.hpp"
#include "../rendering/Sprite.hpp"

class LevelSelectMenu : public Menu
{
public:

    static const int BUTTON_SHOW_DETAILS = 0;
    static const int BUTTON_MAIN_MENU = 1;
    static const int SPRITE_LEVELINFO = 2;
    static const int BUTTON_LEFT = 3;
    static const int BUTTON_RIGHT = 4;
    static const int LABEL_CAPTION = 38;

    LevelSelectMenu(sf::RenderWindow& screen, ResourceManager& resourceManager);

    void setLevelPreview(const Sprite& sprite);
    void hideLeftButton(const bool value);
    void hideRightButton(const bool value);
};

#endif // LEVELSELECTMENU_HPP