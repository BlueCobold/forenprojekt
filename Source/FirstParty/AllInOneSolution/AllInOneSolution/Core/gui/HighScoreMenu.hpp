#pragma once

#ifndef HIGHSCOREMENU_HPP
#define HIGHSCOREMENU_HPP

#include "CaptionMenu.hpp"

class HighScoreMenu : public CaptionMenu
{
public:

    static const int BUTTON_CLOSE = 0;
    static const int LABEL_PLACE_ONE = 11;
    static const int LABEL_POINTS_ONE = 21;

    HighScoreMenu(const sf::Vector2f& position, sf::RenderWindow& screen, ResourceManager& resourceManager);
};

#endif // HIGHSCOREMENU_HPP