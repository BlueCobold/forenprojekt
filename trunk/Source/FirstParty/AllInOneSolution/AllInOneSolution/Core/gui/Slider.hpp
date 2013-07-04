#pragma once

#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "SliderStyle.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Slider
{
public:
    Slider(int id, SliderStyle style, const sf::Vector2f& position, const sf::Vector2f& offset);

    virtual void update(const sf::RenderWindow& screen);

    virtual void draw(const DrawParameter& params);

    int getId() const;

    float getValue() const;

    void setPosition(sf::Vector2f position);

    void Slider::setValue(float value);

private:
    int m_id;

    float m_value;
    float m_min;
    float m_max;

    sf::Vector2f m_position;
    sf::Vector2f m_sliderPosition;

    sf::Vector2f m_offset;

    sf::Sprite* m_spriteSlider;
    sf::Sprite* m_spriteBackround;

    SliderStyle m_style;

    void calculateSliderPosition(sf::IntRect rect);
    void calculateValue(sf::IntRect rect, sf::Vector2i mousePosition);
};

#endif // SLIDER_HPP