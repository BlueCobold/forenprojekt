#pragma once

#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "../rendering/Drawable.hpp"

#include <SFML/Graphics/Sprite.hpp>

class ResourceManager;
class DrawParameter;
namespace sf
{
    class RenderWindow;
};

/// Mouse cursor replacing the system cursor.
class Cursor : public Drawable
{
public:
    Cursor(ResourceManager& resourceManager, const sf::RenderWindow& screen);

    void update();
    void draw(const DrawParameter& params);

private:
    sf::Sprite m_sprite;
    const sf::RenderWindow& m_screen;
};

#endif // CURSOR_HPP
