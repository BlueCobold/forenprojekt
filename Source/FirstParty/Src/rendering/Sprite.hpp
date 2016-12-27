#pragma once

#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "Blending.hpp"
#include "Drawable.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Sprite : public Drawable
{
public:
    Sprite()
    { }

    Sprite(sf::Sprite basicSprite, Blending::Mode blendMode = Blending::RegularAlpha) :
        m_sprite(basicSprite),
        m_blendMode(blendMode)
    { }

    const sf::Sprite& getSprite() const
    {
        return m_sprite;
    }

    void setColor(const sf::Color &color)
    {
        m_sprite.setColor(color);
    }

    void setPosition(float x, float y)
    {
        m_sprite.setPosition(x, y);
    }

    void setPosition(const sf::Vector2f &position)
    {
        m_sprite.setPosition(position);
    }

    const sf::Vector2f& getPosition() const
    {
        return m_sprite.getPosition();
    }

    void setScale(const sf::Vector2f &factors)
    {
        m_sprite.setScale(factors);
    }

    void setScale(float x, float y)
    {
        m_sprite.setScale(x, y);
    }

    void setTextureRect(const sf::IntRect &rectangle)
    {
        m_sprite.setTextureRect(rectangle);
    }

    void setOrigin(float x, float y)
    {
        m_sprite.setOrigin(x, y);
    }
 
    void setOrigin(const sf::Vector2f &origin)
    {
        m_sprite.setOrigin(origin);
    }

    const sf::IntRect& getTextureRect() const
    {
        return m_sprite.getTextureRect();
    }

    sf::FloatRect getGlobalBounds() const
    {
        return m_sprite.getGlobalBounds();
    }

    void setTexture(const sf::Texture &texture, bool resetRect = false)
    {
        m_sprite.setTexture(texture, resetRect);
    }

private:
    void doDraw(const DrawParameter& params) override
    {
        params.getTarget().draw(m_sprite, Blending::toSfmlMode(m_blendMode));
    }

    sf::Sprite m_sprite;
    Blending::Mode m_blendMode;
};

#endif // SPRITE_HPP
