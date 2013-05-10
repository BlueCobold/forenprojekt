#pragma once

#ifndef REPLAY_MENU_HPP
#define REPLAY_MENU_HPP

#include "CaptionMenu.hpp"

class ReplayMenu : public CaptionMenu
{
public:

    static const int BUTTON_PLAY_AGAIN = 0;

    ReplayMenu(const sf::Vector2f& position,
               sf::RenderWindow& screen,
               ResourceManager& resourceManager);
};

#endif // REPLAY_MENU_HPP
