#pragma once

#ifndef SCROLLWINDOW_HPP
#define SCROLLWINDOW_HPP

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "MenuTemplate.hpp"
#include <functional>
#include <vector>
#include <memory>

class DrawParameter;
class Button;
class CheckBox;
class Slider;

class SubWindow
{
private:
    int m_innerHeight;
    sf::Vector2f m_center;
    sf::Vector2f m_position;
    sf::Vector2f m_offset;
    sf::Vector2f m_size;
    sf::Vector2f m_innerPosition;
    sf::View m_orginalView;

    int m_startValue;
    int m_endValue;

    sf::RectangleShape m_windowRect;
    sf::RectangleShape m_sliderRect;
    sf::RectangleShape m_positionRect;

    std::vector<std::unique_ptr<LineLabel>> m_labels;
    std::vector<std::unique_ptr<MenuSprite>> m_sprites;
    std::vector<std::unique_ptr<CheckBox>> m_checkBoxes;
    std::vector<std::unique_ptr<Button>> m_buttons;
    std::vector<std::unique_ptr<Slider>> m_slider;

    void on(const DrawParameter& params);
    void off(const DrawParameter& params);
    void createCheckBox(const CheckBoxInfo& info);
    void createSlider(const SliderInfo& info);
    void createButton(const ButtonInfo& info);
    void createLabel(const LineLabel& info);
    void createSprite(const MenuSprite& info);

    std::function<void(const Button& sender)> m_clickCallback;

    float percentToWindowPixels(float percent = 1.f);
    void setSliderPosition();
    float sliderPixelToWindowPixel(int pixel = 1);
public:
    void draw(const DrawParameter& params);
    void update(const sf::RenderWindow& screen);
    SubWindow(const sf::Vector2f& position,
              const sf::Vector2f& size,
              const sf::Vector2f& innerPosition,
              const sf::Vector2f& offset,
              const int innerHeight,
              const MenuElements& elements);

    void setOffset(const sf::Vector2f& offset);
    void setPosition(const sf::Vector2f& position);
    void setInnerPosition(const sf::Vector2f& position);
};

#endif