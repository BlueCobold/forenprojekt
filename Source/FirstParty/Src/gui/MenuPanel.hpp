#pragma once

#ifndef MENU_PANEL_HPP
#define MENU_PANEL_HPP

#include "MenuTemplate.hpp"
#include "../rendering/Drawable.hpp"
#include "../animation/TimedObject.hpp"

#include <functional>
#include <string>
#include <vector>

class Button;
class CheckBox;
class LineLabel;
class MenuSprite;
class Slider;
class InfoBox;
class AnimationContainer;
class InteractiveLabel;

class MenuPanel : public Drawable
{
public:

    MenuPanel(const std::vector<std::unique_ptr<MenuElement>>& elements);

    void drawAdditionalForeground(const DrawParameter& params);

    virtual void update(const sf::RenderWindow& screen,
        const double time,
        const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0));

    void update(const sf::RenderWindow& screen,
        const double time,
        const MenuElementType::Type type,
        const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0));

    void registerOnClick(std::function<void(const Button& sender)> callback);

    template<class T>
    T* find(int id, const MenuElementType::Type type) const
    {
        for(auto it = begin(m_elements); it != end(m_elements); ++it)
        {
            if(it->get()->getType() == type && it->get()->getId() == id)
                return dynamic_cast<T*>(it->get());
        }
        return nullptr;
    }

    void add(std::unique_ptr<MenuElement> element);
    const std::vector<std::unique_ptr<MenuElement>>& getElements() const;

private:
    std::vector<std::unique_ptr<MenuElement>> m_elements;
    std::function<void(const Button& sender)> m_clickCallback;

    void doDraw(const DrawParameter& params) override;

    void setCorrelation();
};

#endif // MENU_PANEL_HPP
