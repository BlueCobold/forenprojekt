#pragma once

#ifndef HIGHSCOREMENU_HPP
#define HIGHSCOREMENU_HPP

#include "CaptionMenu.hpp"

class HighScoreMenu : public CaptionMenu
{
public:

    static const int BUTTON_CLOSE = 0;
    static const int LABEL_PLACES = 11;
    static const int LABEL_POINTS = 21;
    static const int CHECKBOX_LOCAL_HIGHSCORE = 30;
    static const int CHECKBOX_GLOBAL_HIGHSCORE = 31;
    static const int LABEL_LOADING = 27;

    HighScoreMenu(sf::RenderWindow& screen, ResourceManager& resourceManager);
};

#endif // HIGHSCOREMENU_HPP