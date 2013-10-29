#pragma once

#ifndef MENU_HPP
#define MENU_HPP

#include "MenuPanel.hpp"
#include "MenuTemplate.hpp"
#include "../rendering/Drawable.hpp"
#include <SFML/System/Vector2.hpp>

#include <functional>
#include <string>
#include <vector>

class ResourceManager;
class MenuElement;
class InputBox;

namespace sf
{
    class RenderWindow;
};

/// Base-class for menus
class Menu : public Drawable
{
public:

    Menu(const MenuTemplate& menuTemplate,
         const sf::Vector2f& position,
         sf::RenderWindow& screen);

    virtual ~Menu();

    virtual void draw(const DrawParameter& params) override;

    virtual void update(const sf::RenderWindow& screen);

    virtual void setPosition(const sf::Vector2f& position);

    const sf::Vector2f& getPosition() const;

    const sf::Vector2i& getSize() const;

    void registerOnClick(std::function<void(const Button& sender)> callback);

    void changeIdleSprite(const int id, const sf::Sprite& sprite);
    void changeHoverSprite(const int id, const sf::Sprite& sprite);
    void changePressedSprite(const int id, const sf::Sprite& sprite);

    sf::RenderWindow& getRenderWindow() const;

    CheckBox& getCheckbox(int id) const;
    Slider& getSlider(int id) const;
    LineLabel& getLabel(int id) const;
    Button& getButton(int id) const;
    MenuSprite& getSprite(int id) const;
    InputBox& getInputBox(int id) const;

protected:

    virtual void drawAdditionalBackground(const DrawParameter& params);
    virtual void drawAdditionalForeground(const DrawParameter& params);
private:

    template<class T>
    T* find(int id, const MenuElementType::Type type) const;

    MenuPanel m_panel;
    MenuTemplate m_template;
    sf::Vector2i m_size;
    sf::Vector2f m_position;
    sf::RenderWindow& m_screen;
};

#endif // MENU_HPP
