#pragma once

#ifndef PAUSE_MENU_HPP
#define PAUSE_MENU_HPP

#include "CaptionMenu.hpp"

class PauseMenu : public CaptionMenu
{
public:

    static const int BUTTON_CONTINUE = 0;

    PauseMenu(const sf::Vector2f& position, sf::RenderWindow& screen, ResourceManager& resourceManager);
};

#endif // PAUSE_MENU_HPP
