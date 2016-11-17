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
class SubWindow;

namespace sf
{
    class RenderWindow;
};

/// Base-class for menus
class Menu : public Drawable
{
public:

    Menu(const MenuTemplate& menuTemplate,
         sf::RenderWindow& screen);

    virtual ~Menu();

    void update(sf::RenderWindow& screen, const double time);
    void update(sf::RenderWindow& screen, const double time, const MenuElementType::Type type);

    void registerOnClick(std::function<void(const Button& sender)> callback);

    sf::RenderWindow& getRenderWindow() const;

    CheckBox& getCheckbox(int id) const;
    Slider& getSlider(int id) const;
    LineLabel& getLabel(int id) const;
    InteractiveLabel& getInteractiveLabel(int id) const;
    Button& getButton(int id) const;
    MenuSprite& getSprite(int id) const;
    InputBox& getInputBox(int id) const;
    SubWindow& getSubWindow(int id) const;
    AnimationContainer& getAnimation(int id) const;

    void setZoomFactor(float zoomFactor);

protected:
    void doDraw(const DrawParameter& params) override;

private:
    virtual void drawAdditionalBackground(const DrawParameter& params);
    virtual void drawAdditionalForeground(const DrawParameter& params);
    virtual void updated(sf::RenderWindow& screen, const double time);

    template<class T>
    T* find(int id, const MenuElementType::Type type) const;

    sf::RenderWindow* m_screen;
    MenuPanel m_panel;
    MenuTemplate m_template;
    float m_zoomFactor;
};

#endif // MENU_HPP
