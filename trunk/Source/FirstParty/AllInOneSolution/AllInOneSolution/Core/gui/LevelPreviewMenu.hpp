#pragma once

#ifndef LEVEL_PREVIEW_NMENU_HPP
#define LEVEL_PREVIEW_NMENU_HPP

#include "Menu.hpp"

class Config;

class LevelPreviewMenu : public Menu
{
public:

    static const int BUTTON_START = 0;
    static const int LABEL_LEVELNAME = 1;
    LevelPreviewMenu(const sf::Vector2f& position, sf::RenderWindow& screen, ResourceManager& resourceManager);
    
    void setLevelInfo(const std::string& levelName);
};

#endif // LEVEL_PREVIEW_NMENU_HPP