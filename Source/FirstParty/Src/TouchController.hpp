#ifndef TOUCHCONTROLLER_HPP
#define TOUCHCONTROLLER_HPP

#include <SFML\Graphics\Rect.hpp>

namespace utility
{
    class TouchController
    {
    private:
        sf::FloatRect m_touchArea;
        float m_minValue;
        float m_maxValue;
        float m_lastValue;
        bool m_horizontal;

    public:
        TouchController(const sf::FloatRect& touchArea,
                        float minValue,
                        float maxValue,
                        bool horizontal = false);

        bool isTouched() const;

        float getValue();

        const sf::FloatRect& getTouchArea() const;

        void setTouchArea(const sf::FloatRect& touchArea);
    };

    extern TouchController TeeterController;
}
#endif