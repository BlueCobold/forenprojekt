#pragma once

#ifndef GRAPHICAL_OBJECT_HPP
#define GRAPHICAL_OBJECT_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace sf
{
    class Texture;
}

/// This class will be used to give a Object a
/// binding to a Graphic
class GraphicalObject
{
public:

    void bindTexture(const sf::Texture& texture, const sf::IntRect& textureRect)
    {
        m_sprite.setTexture(texture);
        m_sprite.setTextureRect(textureRect);
    }

    void bindTexture(const sf::Texture& texture)
    {
        m_sprite.setTexture(texture);
    }

protected:
    
    sf::Sprite m_sprite;
    
};

#endif // GRAPHICAL_OBJECT_HPP
