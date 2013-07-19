#pragma once

#ifndef GOODYHUD_HPP
#define GOODYHUD_HPP

#include "HUDElement.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

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
private:
    sf::Sprite m_icon;
};

#endif // GOODY_HPP
