#pragma once

#include <SFML/Graphics/Sprite.hpp>

class GraphicalObject
{
public:

    void bindTexture(sf::Texture& texture, sf::IntRect textureRect)
    {
        m_sprite.setTexture(texture);
        m_sprite.setTextureRect(textureRect);
    }

    void bindTexture(sf::Texture& texture)
    {
        m_sprite.setTexture(texture);
    }

protected:
    
    sf::Sprite m_sprite;
    
};