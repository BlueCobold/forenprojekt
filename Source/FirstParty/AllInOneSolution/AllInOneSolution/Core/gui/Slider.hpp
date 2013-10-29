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
    Slider(const int id, const SliderStyle style, const sf::Vector2f& position, const sf::Vector2f& offset);

    virtual void update(const sf::RenderWindow& screen, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0)) override;

    virtual void draw(const DrawParameter& params) override;

    float getValue() const;

    void Slider::setValue(const float value);

protected:

    virtual void onPositionChanged() override;

private:

    bool m_active;

    float m_value;
    float m_min;
    float m_max;
    int m_pick;

    sf::Vector2f m_sliderPosition;

    sf::Sprite* m_spriteSlider;
    sf::Sprite* m_spriteBackground;

    SliderStyle m_style;

    void calculateSliderPosition(const sf::IntRect& rect);
    void calculateValue(const int left, const int mousex);
};

#endif // SLIDER_HPP
