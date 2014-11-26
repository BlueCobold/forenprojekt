#pragma once

#ifndef NEWHIGHSCOREMENU_HPP
#define NEWHIGHSCOREMENU_HPP

#include "CaptionMenu.hpp"

class NewHighScoreMenu : public Menu
{
public:
    static const int INPUTBOX = 1;
    static const int BUTTON_SKIP = 5;
    static const int BUTTON_OK = 6;

    NewHighScoreMenu(const sf::Vector2f& position, sf::RenderWindow& screen, ResourceManager& resourceManager);
};

#endif // NEWHIGHSCOREMENU_HPP