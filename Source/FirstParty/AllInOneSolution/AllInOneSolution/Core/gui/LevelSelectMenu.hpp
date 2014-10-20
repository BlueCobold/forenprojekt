#pragma once

#ifndef LEVELSELECTMENU_HPP
#define LEVELSELECTMENU_HPP

#include "CaptionMenu.hpp"
#include <SFML/Graphics/Rect.hpp>

class LevelSelectMenu : public CaptionMenu
{
public:

    static const int BUTTON_SHOW_DETAILS = 0;
    static const int BUTTON_MAIN_MENU = 1;
    static const int SPRITE_LEVELINFO = 2;
    static const int BUTTON_LEFT = 3;
    static const int BUTTON_RIGHT = 4;

    LevelSelectMenu(const sf::Vector2f& position, sf::RenderWindow& screen, ResourceManager& resourceManager);

    void setLevelPreview(const sf::Texture& texture, const sf::IntRect& textureRect);
    void hideLeftButton(const bool value);
    void hideRightButton(const bool value);
};

#endif // LEVELSELECTMENU_HPP