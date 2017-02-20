#pragma once

#ifndef ACHIEVEMENTPOPUPMENU_HPP
#define ACHIEVEMENTPOPUPMENU_HPP

#include "Menu.hpp"
#include <SFML/Graphics/Rect.hpp>

class AchievementPopUpMenu : public Menu
{
public:

    static const int LABEL_ACHIEVEMENT_NAME = 1;

    AchievementPopUpMenu(sf::RenderWindow& screen, ResourceManager& resourceManager);
};

#endif // ACHIEVEMENTPOPUPMENU_HPP