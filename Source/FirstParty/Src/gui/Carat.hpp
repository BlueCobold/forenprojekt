#pragma once

#ifndef CARAT_HPP
#define CARAT_HPP

#include "../rendering/Drawable.hpp"

#include <SFML/Graphics/Sprite.hpp>

class ResourceManager;
class DrawParameter;

namespace sf
{
    class RenderWindow;
};

class Caret
{
private:
    sf::Sprite m_sprite;
    const float m_blinkTime;
    float m_nextFullFadeOut;
    sf::Vector2f m_offset;
    bool m_activated;

public:
    Caret(float frequency, const sf::Vector2f& offset, const sf::Sprite& sprite);

    void update(const float time);
    void draw(const DrawParameter& params);

    void setPosition(const sf::Vector2f& position);

    void enable();
    void disable();

    bool isActive();
};

#endif
