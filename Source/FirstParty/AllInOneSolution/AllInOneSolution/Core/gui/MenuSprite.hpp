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
    MenuSprite(sf::Vector2f position, sf::Vector2f offset, int id);

    void setOffset(sf::Vector2f offset);
    void setPosition(sf::Vector2f offset);
    void setId(int id);

    sf::Vector2f getOffset();
    sf::Vector2f getPosition();
    int getId();

    void draw(const DrawParameter& params);

private:

    int m_id;
};

#endif