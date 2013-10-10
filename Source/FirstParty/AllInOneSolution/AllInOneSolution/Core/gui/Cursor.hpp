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
    Cursor(const sf::RenderWindow& screen);

    void setResourceManager(ResourceManager& resourceManager);

    void update();
    void draw(const DrawParameter& params);

private:
    sf::Sprite m_sprite;
    const sf::RenderWindow& m_screen;
};

#endif // CURSOR_HPP
