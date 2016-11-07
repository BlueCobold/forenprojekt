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

class Caret : public Drawable
{
private:
    sf::Sprite m_sprite;
    const double m_blinkTime;
    double m_nextFullFadeOut;
    sf::Vector2f m_offset;
    bool m_activated;

    void doDraw(const DrawParameter& params) override;

public:
    Caret(float frequency, const sf::Vector2f& offset, const sf::Sprite& sprite);

    void update(const double time);

    void setPosition(const sf::Vector2f& position);

    void enable();
    void disable();

    bool isActive();
};

#endif
