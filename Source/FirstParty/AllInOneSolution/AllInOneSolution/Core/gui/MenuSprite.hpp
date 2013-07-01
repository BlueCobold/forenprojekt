#pragma once

#ifndef MENUSPRITE_HPP
#define MENUSPRITE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include "../rendering/DrawParameter.hpp"

class MenuSprite : public sf::Sprite
{
public:
    sf::Vector2f m_offset;
    sf::Vector2f m_position;
    
    MenuSprite();

    void setOffset(sf::Vector2f offset);

    void setPosition(sf::Vector2f offset);

    sf::Vector2f getOffset();

    sf::Vector2f getPosition();

    void draw(const DrawParameter& params);
};

#endif