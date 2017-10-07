#pragma once

#ifndef SCREEN_SIZE_HPP
#define SCREEN_SIZE_HPP

#include <SFML/System/Vector2.hpp>

#include <algorithm>

class ScreenSize
{
public:
    enum ScaleType
    {
        XY,
        MIN,
        MAX
    };

    explicit ScreenSize(const sf::Vector2f& sizeOffset = sf::Vector2f(0, 0),
                        const sf::Vector2f& relativeSize = sf::Vector2f(0, 0),
                        float aspectRatio = 0,
                        ScaleType type = XY) :
        m_relativeSize(relativeSize),
        m_offset(sizeOffset),
        m_aspectRatio(aspectRatio),
        m_scaleType(type)
    { }

    void setScreenSize(const sf::Vector2f& screenSize)
    {
        m_screenSize = screenSize;
        calculateSize();
    }

    const sf::Vector2f& getCurrentSize() const
    {
        return m_currentSize;
    }

    const sf::Vector2f& getFixedSize() const
    {
        return m_offset;
    }

    float getFixedAspectRatio() const
    {
        return m_aspectRatio;
    }

private:
    sf::Vector2f m_relativeSize;
    sf::Vector2f m_offset;
    float m_aspectRatio;
    ScaleType m_scaleType;
    sf::Vector2f m_screenSize;
    sf::Vector2f m_currentSize;

    void calculateSize()
    {
        switch(m_scaleType)
        {
            case XY:
                if(m_aspectRatio != 0)
                    m_currentSize = sf::Vector2f(m_screenSize.x * m_relativeSize.x,
                                                 m_aspectRatio * m_screenSize.x * m_relativeSize.x)
                                  + m_offset;
                else
                    m_currentSize = sf::Vector2f(m_screenSize.x * m_relativeSize.x,
                                                 m_screenSize.y * m_relativeSize.y)
                                  + m_offset;
                break;
            case MIN:
            {
                if(m_aspectRatio * m_screenSize.x * m_relativeSize.x <= m_screenSize.y * m_relativeSize.y)
                    m_currentSize = sf::Vector2f(m_screenSize.x * m_relativeSize.x,
                                                 m_aspectRatio * m_screenSize.x * m_relativeSize.x)
                                  + m_offset;
                else
                    m_currentSize = sf::Vector2f(m_screenSize.y * m_relativeSize.y / m_aspectRatio,
                                                 m_screenSize.y * m_relativeSize.y)
                                  + m_offset;
                break;
            }
            case MAX:
            {
                if(m_aspectRatio * m_screenSize.x * m_relativeSize.x > m_screenSize.y * m_relativeSize.y)
                    m_currentSize = sf::Vector2f(m_screenSize.x * m_relativeSize.x,
                                                 m_aspectRatio * m_screenSize.x * m_relativeSize.x)
                                  + m_offset;
                else
                    m_currentSize = sf::Vector2f(m_screenSize.y * m_relativeSize.y / m_aspectRatio,
                                                 m_screenSize.y * m_relativeSize.y)
                                  + m_offset;
                break;
            }
        }
    }
};

#endif
