#pragma once

#ifndef GOODYHUD_HPP
#define GOODYHUD_HPP

#include "HUDElement.hpp"
#include "../LineLabel.hpp"
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
             const sf::Vector2f& offset);

    void update(const DrawParameter& params);

    void draw(const DrawParameter& params);

    void updateState(const Goody& goody);
private:
    static const int Deselected = 0;
    static const int Selected = 1;
    static const int Active = 2;
    static const int Disabled = 3;
    static const int DisabledSelected = 4;

    sf::Sprite m_icon;
    sf::IntRect m_startRect;
    int m_iconHeight;

    bool m_selected;
    bool m_disabled;
    bool m_active;

    LineLabel m_charges;
};

#endif // GOODY_HPP
