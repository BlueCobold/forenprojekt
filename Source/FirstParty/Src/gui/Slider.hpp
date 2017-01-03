#pragma once

#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "MenuElement.hpp"
#include "SliderStyle.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Slider : public MenuElement
{
public:
    Slider(const int id, const SliderStyle style, const ScreenLocation& position);

    float getValue() const;
    void setValue(const float value);

protected:

private:
    std::unique_ptr<MenuElement> doClone() const override;
    void doDraw(const DrawParameter& params) override;
    void updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0)) override;
    void layoutUpdated(const sf::Vector2f& screenSize) override;

    SliderStyle m_style;

    bool m_active;
    float m_value;
    float m_min;
    float m_max;
    int m_pick;

    sf::Vector2f m_buttonPosition;
    Sprite* m_button;
    Sprite* m_background;

    void calculateSliderPosition(const sf::IntRect& rect);
    void calculateValue(const int left, const int mousex);
};

#endif // SLIDER_HPP
