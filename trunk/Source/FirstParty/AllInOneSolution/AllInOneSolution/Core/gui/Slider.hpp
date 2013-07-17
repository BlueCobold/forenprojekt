#pragma once

#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "../rendering/Drawable.hpp"
#include "SliderStyle.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Slider : public Drawable
{
public:
    Slider(const int id, const SliderStyle style, const sf::Vector2f& position, const sf::Vector2f& offset);

    virtual void update(const sf::RenderWindow& screen);

    virtual void draw(const DrawParameter& params) override;

    int getId() const;

    float getValue() const;

    void setPosition(const sf::Vector2f& position);

    void Slider::setValue(const float value);

private:
    bool m_active;
    int m_id;

    float m_value;
    float m_min;
    float m_max;
    int m_pick;

    sf::Vector2f m_position;
    sf::Vector2f m_sliderPosition;

    sf::Vector2f m_offset;

    sf::Sprite* m_spriteSlider;
    sf::Sprite* m_spriteBackround;

    SliderStyle m_style;

    void calculateSliderPosition(const sf::IntRect& rect);
    void calculateValue(const int left, const int mousex);
};

#endif // SLIDER_HPP
