#pragma once

#ifndef TOUCHBUTTON_HPP
#define TOUCHBUTTON_HPP

#include <SFML\Graphics\Rect.hpp>

class TouchButton
{
private:
    sf::FloatRect m_touchArea;

    bool m_newlyPressed;
    bool m_stillPressed;
    bool m_released;

public:
    TouchButton(const sf::FloatRect& touchArea);

    bool isDown();
    bool isPressed();
    bool isReleased();
    void update();

    void setTouchArea(const sf::FloatRect& touchArea);
};

#endif