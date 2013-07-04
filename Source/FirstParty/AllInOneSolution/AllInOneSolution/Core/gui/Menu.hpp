#pragma once

#ifndef MENU_HPP
#define MENU_HPP

#include "../rendering/Drawable.hpp"
#include "Button.hpp"
#include "MenuTemplate.hpp"
#include "CheckBox.hpp"
#include "Slider.hpp"
#include "../resources/Config.hpp"

#include <SFML/System/Vector2.hpp>

#include <functional>
#include <vector>

class ResourceManager;

namespace sf
{
    class RenderWindow;
};

/// Base-class for menus
class Menu : public Drawable
{
protected:

    void createButton(const ButtonInfo& info);
    void createCheckBox(const CheckBoxInfo& info);
    void createSlider(const SliderInfo& info);
    void createLabel(const LineLabel& info);
    void createSprite(const MenuSprite& info);

    CheckBox& getCheckboxes(int id);
    Slider& getSlider(int id);
    LineLabel& getLabel(int id);
    sf::RenderWindow& getRenderWindow();
    MenuSprite& getSprite(int id);

    MenuTemplate& getTemplate();

private:

    MenuTemplate m_template;
    std::vector<std::unique_ptr<Button>> m_buttons;
    std::vector<std::unique_ptr<LineLabel>> m_labels;
    std::vector<std::unique_ptr<MenuSprite>> m_sprites;
    std::vector<std::unique_ptr<CheckBox>> m_checkBoxes;
    std::vector<std::unique_ptr<Slider>> m_slider;
    sf::Vector2i m_size;
    sf::Vector2f m_position;
    sf::RenderWindow& m_screen;
    std::function<void(const Button& sender)> m_clickCallback;

public:

    Menu(const MenuTemplate& menuTemplate,
         const sf::Vector2f& position,
         sf::RenderWindow& screen);

    virtual ~Menu();

    virtual void draw(const DrawParameter& params);

    virtual void update(const sf::RenderWindow& screen);

    virtual void setPosition(const sf::Vector2f& position);

    const sf::Vector2f& getPosition() const;

    const sf::Vector2i& getSize() const;

    void registerOnClick(std::function<void(const Button& sender)> callback);
};

#endif // MENU_HPP
