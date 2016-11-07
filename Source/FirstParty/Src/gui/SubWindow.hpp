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

struct SubWindowStyle
{
    sf::Sprite scrollbarTop;
    sf::Sprite scrollbarMiddle;
    sf::Sprite scrollbarBottom;
};

class SubWindow : public MenuElement
{
    std::unique_ptr<MenuElement> doClone() const override;

public:

    SubWindow(const int id,
              const sf::Vector2f& position,
              const sf::Vector2f& size,
              const sf::Vector2f& offset,
              const int innerHeight,
              const std::vector<std::unique_ptr<MenuElement>>& elements,
              const SubWindowStyle& style);

    MenuPanel* getPanel();

    int getInnerHeight() const;
    void setInnerHeight(int innerHeight);
    void setZoomFactor(float zoomFactor);

    void setLanguage(const std::string& language) override;

private:
    MenuPanel m_panel;
    SubWindowStyle m_style;
    int m_innerHeight;
    sf::Vector2f m_center;
    sf::Vector2f m_size;
    sf::View m_orginalView;
    float m_zoomFactor;

    int m_startValue;
    int m_endValue;
    bool m_active;

    sf::RectangleShape m_windowRect;
    sf::RectangleShape m_sliderRect;
    sf::RectangleShape m_positionRect;

    sf::Vector2i m_cursorPosition;

    void on(const DrawParameter& params);
    void off(const DrawParameter& params);
    void doDraw(const DrawParameter& params) override;
    void onDrawAdditionalForeground(const DrawParameter& params) override;
    void updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0)) override;
    void layoutUpdated(const sf::Vector2f& screenSize) override;

    std::function<void(const Button& sender)> m_clickCallback;

    float sliderPixelToWindowPixel(float pixel);
    float windowPixelToSliderPixel(float pixel);

    sf::Vector2i getMouseOffset(const sf::RenderWindow& screen);
};

#endif
