#pragma once

#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "../rendering/Drawable.hpp"
#include "../rendering/Sprite.hpp"

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

private:
    Sprite m_sprite;
    const sf::RenderWindow& m_screen;

    void doDraw(const DrawParameter& params) override;
};

#endif // CURSOR_HPP
