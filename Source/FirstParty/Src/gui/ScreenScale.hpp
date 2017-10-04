#pragma once

#ifndef SCREEN_SCALE_HPP
#define SCREEN_SCALE_HPP

#include <SFML/System/Vector2.hpp>

class ScreenScale
{
public:
    enum ScaleType
    {
        XY,
        X_FOR_BOTH,
        Y_FOR_BOTH,
        MIN,
        MAX
    };

    explicit ScreenScale(const sf::Vector2f& weightFactor = sf::Vector2f(0, 0),
                         const sf::Vector2f& referenceSize = sf::Vector2f(1, 1),
                         const sf::Vector2f& postScale = sf::Vector2f(1, 1),
                         ScaleType scaleType = XY) :
        m_weightFactor(weightFactor),
        m_screenSize(sf::Vector2f(1, 1)),
        m_referenceSize(referenceSize),
        m_currentScale(sf::Vector2f(1, 1)),
        m_postScale(postScale),
        m_scaleType(scaleType)
    { }

    void setScreenSize(const sf::Vector2f& screenSize)
    {
        m_screenSize = screenSize;
        calculateScale();
    }

    const sf::Vector2f& getCurrentScale() const
    {
        return m_currentScale;
    }

private:
    sf::Vector2f m_weightFactor;
    sf::Vector2f m_screenSize;
    sf::Vector2f m_referenceSize;
    sf::Vector2f m_currentScale;
    sf::Vector2f m_postScale;
    ScaleType m_scaleType;

    float getX()
    {
        return m_postScale.x * (1 - m_weightFactor.x + m_weightFactor.x * m_screenSize.x / m_referenceSize.x);
    }

    float getY()
    {
        return m_postScale.y * (1 - m_weightFactor.y + m_weightFactor.y * m_screenSize.y / m_referenceSize.y);
    }

    void calculateScale()
    {
        switch(m_scaleType)
        {
            case XY:
                m_currentScale = sf::Vector2f(getX(), getY());
                break;
            case X_FOR_BOTH:
            {
                auto scale = getX();
                m_currentScale = sf::Vector2f(scale, scale);
                break;
            }
            case Y_FOR_BOTH:
            {
                auto scale = getY();
                m_currentScale = sf::Vector2f(scale, scale);
                break;
            }
            case MIN:
            {
                auto scale = std::min(getX(), getY());
                m_currentScale = sf::Vector2f(scale, scale);
                break;
            }
            case MAX:
            {
                auto scale = std::max(getX(), getY());
                m_currentScale = sf::Vector2f(scale, scale);
                break;
            }
        }
    }
};

#endif
