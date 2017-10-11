#include "TouchController.hpp"
#include "Input.hpp"

namespace utility
{
    TouchController::TouchController(const sf::FloatRect& touchArea,
                                     float minValue,
                                     float maxValue,
                                     bool horizontal) :
        m_touchArea(touchArea),
        m_minValue(minValue),
        m_maxValue(maxValue),
        m_lastValue(0),
        m_currentValue(0),
        m_interpolatedValue(0),
        m_horizontal(horizontal)
    { }

    bool TouchController::isTouched() const
    {
#if defined(IOS) || defined(ANDROID)
        return m_touchArea.contains(sf::Vector2f(utility::Mouse.getTouchPosition()));
#elif defined(TOUCHSIM)
        return m_touchArea.contains(sf::Vector2f(utility::Mouse.getRealWindowPosition()));
#else
        return false;
#endif

    }

    void TouchController::startInterpolation()
    {
        getValue(false);
    }

    void TouchController::interpolate(int steps, int current)
    {
        m_interpolatedValue = m_lastValue + current * (m_currentValue - m_lastValue) / steps;
    }

    float TouchController::getValue(bool interpolated)
    {
        if(interpolated)
            return m_interpolatedValue;

        float zeroPointDistance = 0;
        float touchPoint = 0;

        if(m_horizontal)
        {
            zeroPointDistance = m_touchArea.width / 2.f;
            if(isTouched())
            {
    #ifdef TOUCHSIM
                touchPoint = m_touchArea.left + zeroPointDistance - utility::Mouse.getRealWindowPosition().x;
    #elif defined(IOS) || defined(ANDROID)
                touchPoint = m_touchArea.left + zeroPointDistance - utility::Mouse.getTouchPosition().x;
    #endif
            }
        }
        else
        {
            zeroPointDistance = m_touchArea.height / 2.f;
            if(isTouched())
            {
    #ifdef TOUCHSIM
                touchPoint = m_touchArea.top + zeroPointDistance - utility::Mouse.getRealWindowPosition().y;
    #elif defined(IOS) || defined(ANDROID)
                touchPoint = m_touchArea.top + zeroPointDistance - utility::Mouse.getTouchPosition().y;
    #endif
            }
        }

        m_lastValue = m_currentValue;
        if(touchPoint > 0)
            m_currentValue = touchPoint / zeroPointDistance * m_maxValue;

        if(touchPoint < 0)
            m_currentValue = touchPoint / zeroPointDistance * m_minValue;

        return m_lastValue;
    }

    const sf::FloatRect& TouchController::getTouchArea() const
    {
        return m_touchArea;
    }

    void TouchController::setTouchArea(const sf::FloatRect& touchArea)
    {
        m_touchArea = touchArea;
    }
}