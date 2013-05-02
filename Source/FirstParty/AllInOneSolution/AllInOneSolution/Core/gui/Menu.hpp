#pragma once

#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/System/Vector2.hpp>

class ResourceManager;
class DrawParameter;

namespace sf
{
    class RenderWindow;
}

/// Basis class for Menu
class Menu
{
protected:

    sf::RenderWindow& m_screen;

    ResourceManager& m_resourceManager;

    sf::Vector2i m_size;

    sf::Vector2f m_position;

public:

    Menu(sf::Vector2f position, sf::RenderWindow& screen, ResourceManager& resourceManager);

    virtual ~Menu();

    virtual void draw(const DrawParameter& params) = 0;

    virtual void update(const sf::RenderWindow& screen) = 0;
};

#endif // MENU_HPP