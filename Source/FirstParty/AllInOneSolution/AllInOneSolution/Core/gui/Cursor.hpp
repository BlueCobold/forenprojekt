#pragma once

#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "../resources/ResourceManager.hpp"
#include "../rendering/DrawParameter.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

/// Mouse cursor replacing the system cursor.
class Cursor : public Drawable
{
public:
    static const std::string TEXTURE_NAME;
    static const int SPRITE_POSITION_X;
    static const int SPRITE_POSITION_Y;
    static const int SPRITE_WIDTH;
    static const int SPRITE_HEIGHT;

    Cursor(ResourceManager& resourceManager, const sf::RenderWindow& screen);

    void update();
    void draw(const DrawParameter& params);

private:
    sf::Sprite m_sprite;
    const sf::RenderWindow& m_screen;
};

#endif // CURSOR_HPP
