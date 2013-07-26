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

    GoodyHUD(ResourceManager& resourceManager,
              const std::string& iconKey,
              const sf::IntRect& textureRect,
              const sf::Vector2f& position,
              float hReference = HUDElement::Left,
              float vReference = HUDElement::Top);

    void update(const DrawParameter& params);

    void draw(const DrawParameter& params);

    void updateState(const Goody &goody);
private:
    static const int Deselected = 0;
    static const int Active = 1;
    static const int Selected = 2;
    static const int Disabled = 3;
    static const int DisabledSelected = 4;


    sf::Sprite m_icon;
    sf::IntRect m_startRect;
    int m_iconHeight;

    bool m_selected;
    bool m_disabled;
    bool m_active;
};

#endif // GOODY_HPP
