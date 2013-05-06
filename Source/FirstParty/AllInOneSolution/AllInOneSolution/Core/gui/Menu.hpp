#pragma once

#ifndef MENU_HPP
#define MENU_HPP

#include "../rendering/Drawable.hpp"
#include "Button.hpp"

#include <SFML/System/Vector2.hpp>

#include <functional>
#include <vector>

class ResourceManager;

namespace sf
{
    class RenderWindow;
}

/// Base-class for menus
class Menu : public Drawable
{
protected:

    sf::RenderWindow& m_screen;

    ResourceManager& m_resourceManager;
    
    void createButton(const sf::Vector2f& position, const sf::Vector2f& offset, const std::string& text, const int id);

private:

    std::vector<std::unique_ptr<Button>> m_buttons;
    sf::Sprite m_background;
    sf::Vector2i m_size;
    sf::Vector2f m_position;
    std::function<void(const Button& sender)> m_clickCallback;

public:

    Menu(const sf::Vector2f& position,
         sf::RenderWindow& screen,
         ResourceManager& resourceManager,
         const std::string& backgroundName);

    virtual ~Menu();

    virtual void draw(const DrawParameter& params);

    virtual void update(const sf::RenderWindow& screen);

    virtual void setPosition(const sf::Vector2f& position);

    const sf::Vector2f& getPosition() const;

    const sf::Vector2i& getSize() const;

    void registerOnClick(std::function<void(const Button& sender)> callback);
};

#endif // MENU_HPP
