#pragma once

#ifndef GRAPHICAL_OBJECT_HPP
#define GRAPHICAL_OBJECT_HPP

#include <SFML/Graphics.hpp>

/// This class will be used to give a Object a
/// binding to a Graphic
class GraphicalObject : public sf::Drawable
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

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

protected:
    
    sf::Sprite m_sprite;
};

#endif // GRAPHICAL_OBJECT_HPP
