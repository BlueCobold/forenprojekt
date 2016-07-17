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
    static const int LABEL_BALLS = 3;
    static const int LABEL_TIME = 5;
    static const int BUTTON_HIGHSCORE = 9;
    // Unused since the coins icon was changed into a button
    //static const int SPRITE_COINS = 6;
    static const int BUTTON_COINS = 8;
    static const int BUTTON_MAIN_MENU = 7;
    static const int CHECKBOX_TIMEATTACKMODE = 10;
    static const int CHECKBOX_TARGETMODE = 11;

    LevelPreviewMenu(sf::RenderWindow& screen, ResourceManager& resourceManager);
    
    void setLevelInfo(const std::string& levelName,
                      const float time,
                      const int balls,
                      const std::string& language);
    void setCoinToolTipText(const std::string& text, const std::string& coins);
};

#endif // LEVEL_PREVIEW_NMENU_HPP