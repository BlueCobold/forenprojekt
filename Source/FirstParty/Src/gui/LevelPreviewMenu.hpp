#pragma once

#ifndef LEVEL_PREVIEW_NMENU_HPP
#define LEVEL_PREVIEW_NMENU_HPP

#include "Menu.hpp"
#include "../rendering/Sprite.hpp"

class Config;

class LevelPreviewMenu : public Menu
{
public:

    static const int BUTTON_START = 0;
    static const int LABEL_LEVELNAME = 1;
    static const int SPRITE_LEVELINFO = 2;
    static const int LABEL_BALLS_HINT = 2;
    static const int LABEL_BALLS = 3;
    static const int LABEL_TIME_HINT = 4;
    static const int LABEL_TIME = 5;
    static const int LABEL_LOADING = 6;
    static const int BUTTON_HIGHSCORE = 9;
    static const int BUTTON_LEFT = 30;
    static const int BUTTON_RIGHT = 40;
    static const int BUTTON_COINS = 8;
    static const int BUTTON_MAIN_MENU = 7;
    static const int BUTTON_OPTIONS = 13;
    static const int CHECKBOX_TARGETMODE = 10;

    LevelPreviewMenu(sf::RenderWindow& screen, ResourceManager& resourceManager);
    
    void setLevelPreview(const Sprite& sprite);
    void setLevelInfo(const std::string& levelName,
                      const double time,
                      const int balls);
    void setCoinToolTipText(const std::string& text, const std::string& coins);
    void setLoading(bool loading);
};

#endif // LEVEL_PREVIEW_NMENU_HPP