#pragma once

#ifndef SCROLLWINDOW_HPP
#define SCROLLWINDOW_HPP

#include "MenuElement.hpp"
#include "MenuPanel.hpp"
#include "MenuTemplate.hpp"

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <functional>
#include <vector>
#include <memory>

class Button;
class CheckBox;
class LineLabel;
class MenuSprite;
class Slider;

class SubWindow : public MenuElement
{
public:

    SubWindow(const sf::Vector2f& position,
              const sf::Vector2f& size,
              const sf::Vector2f& innerPosition,
              const sf::Vector2f& offset,
              const int innerHeight,
              const MenuElements& elements);

    virtual void draw(const DrawParameter& params) override;
    virtual void update(const sf::RenderWindow& screen) override;

    void setInnerPosition(const sf::Vector2f& position);

protected:

    virtual void onPositionChanged() override;

private:

    MenuPanel m_panel;
    int m_innerHeight;
    sf::Vector2f m_center;
    sf::Vector2f m_size;
    sf::Vector2f m_innerPosition;
    sf::View m_orginalView;

    int m_startValue;
    int m_endValue;
    bool m_active;

    sf::RectangleShape m_windowRect;
    sf::RectangleShape m_sliderRect;
    sf::RectangleShape m_positionRect;

    void on(const DrawParameter& params);
    void off(const DrawParameter& params);

    std::function<void(const Button& sender)> m_clickCallback;

    float percentToWindowPixels(float percent = 1.f);
    void setSliderPosition();
    float sliderPixelToWindowPixel(int pixel = 1);
};

#endif
