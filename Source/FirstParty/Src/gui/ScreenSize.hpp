#pragma once

#ifndef SCREEN_SIZE_HPP
#define SCREEN_SIZE_HPP

#include <SFML/System/Vector2.hpp>

class ScreenSize
{
public:
    explicit ScreenSize(const sf::Vector2f& fixedSize = sf::Vector2f(0 ,0),
                        const sf::Vector2f& relativeSize = sf::Vector2f(0 ,0)) :
        m_relativeSize(relativeSize),
        m_offset(fixedSize)
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

private:
    sf::Vector2f m_relativeSize;
    sf::Vector2f m_offset;
    sf::Vector2f m_screenSize;
    sf::Vector2f m_currentSize;

    void calculateSize()
    {
        m_currentSize = sf::Vector2f(m_screenSize.x * m_relativeSize.x, 
                                    m_screenSize.y * m_relativeSize.y)
                      + m_offset;
    }
};

#endif
