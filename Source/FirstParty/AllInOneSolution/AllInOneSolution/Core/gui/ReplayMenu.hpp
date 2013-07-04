#pragma once

#ifndef REPLAY_MENU_HPP
#define REPLAY_MENU_HPP

#include "CaptionMenu.hpp"

class ReplayMenu : public CaptionMenu
{
public:

    static const int BUTTON_PLAY_AGAIN = 0;
    static const int LABEL_POINTS = 1;
    static const int SPRITE_SYMBOL = 3;

    ReplayMenu(const sf::Vector2f& position,
               sf::RenderWindow& screen,
               ResourceManager& resourceManager);

    void setPoints(int points);
    void setSymbol(ResourceManager& resourceManager, sf::IntRect textureRect, std::string textureKey = "");

};

#endif // REPLAY_MENU_HPP
