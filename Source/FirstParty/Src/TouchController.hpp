#pragma once

#ifndef TOUCHCONTROLLER_HPP
#define TOUCHCONTROLLER_HPP

#include <SFML/Graphics/Rect.hpp>

namespace utility
{
    class TouchController
    {
    private:
        sf::FloatRect m_touchArea;
        float m_minValue;
        float m_maxValue;
        float m_lastValue;
        float m_currentValue;
        float m_interpolatedValue;
        bool m_horizontal;

    public:
        TouchController(const sf::FloatRect& touchArea,
                        float minValue,
                        float maxValue,
                        bool horizontal = false);

        bool isTouched() const;

        float getValue(bool interpolated = true);
        void startInterpolation();
        void interpolate(int steps, int current);

        const sf::FloatRect& getTouchArea() const;

        void setTouchArea(const sf::FloatRect& touchArea);
    };
}
#endif