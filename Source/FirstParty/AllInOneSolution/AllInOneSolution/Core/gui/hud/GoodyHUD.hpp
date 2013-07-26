#pragma once

#ifndef GOODYHUD_HPP
#define GOODYHUD_HPP

#include "HUDElement.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

class Goody;

class GoodyHUD : public HUDElement
{
public:
    enum State {Disabled,
                Active,
                Selected,
                Deselected
                };

    GoodyHUD(ResourceManager& resourceManager,
              const std::string& iconKey,
              const sf::IntRect& textureRect,
              const sf::Vector2f& position,
              const int goodyIconHeight,
              float hReference = HUDElement::Left,
              float vReference = HUDElement::Top);

    void update(const DrawParameter& params);

    void draw(const DrawParameter& params);

    void updateGoodyState(const Goody &goody);
private:
    sf::Sprite m_icon;
    sf::Sprite m_activeIcon;
    sf::Sprite m_selectedIcon;
    sf::Sprite m_disabledIcon;
    State m_goodyState;
};

#endif // GOODY_HPP
