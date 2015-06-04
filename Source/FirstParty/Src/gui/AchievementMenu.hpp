#pragma once

#ifndef ACHIEVEMENTMENU_HPP
#define ACHIEVEMENTMENU_HPP

#include "Menu.hpp"
#include <SFML/Graphics/Rect.hpp>

class AchievementMenu : public Menu
{
public:

    static const int BUTTON_MAIN_MENU = 1;
    static const int BUTTON_RIGHT = 2;
    static const int BUTTON_LEFT = 3;
    static const int LABEL_ACHIEVEMENT_NAME = 4;
    static const int LABEL_ACHIEVEMENT_COUNTER = 5;

    AchievementMenu(sf::RenderWindow& screen, ResourceManager& resourceManager);

    void setLevelPreview(const sf::Texture& texture, const sf::IntRect& textureRect);
    void hideLeftButton(const bool value);
    void hideRightButton(const bool value);
};

#endif // ACHIEVEMENTMENU_HPP