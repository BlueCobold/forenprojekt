#pragma once

#ifndef SCREENLOCATION_HPP
#define SCREENLOCATION_HPP

#include <SFML/System/Vector2.hpp>

class ScreenLocation
{
public:
    static const float Left;
    static const float Right;
    static const float Center;
    static const float Top;
    static const float Bottom;

private:
    sf::Vector2f m_relativePosition;
    sf::Vector2f m_relativeAxisInvertedPosition;
    sf::Vector2f m_offset;
    sf::Vector2f m_screenSize;
    sf::Vector2f m_currentPosition;

    void calculatePosition();
public:
    explicit ScreenLocation(const sf::Vector2f& relativePosition = sf::Vector2f(0 ,0),
                            const sf::Vector2f& offset = sf::Vector2f(0 ,0),
                            const sf::Vector2f& screenSize = sf::Vector2f(0 ,0),
                            const sf::Vector2f& relativeAxisInvertedPosition = sf::Vector2f(0 ,0));

    ScreenLocation& addOffset(const sf::Vector2f& offset);

    void setScreenSize(const sf::Vector2f& screenSize);

    void setOffset(const sf::Vector2f& offset);

    void setPosition(const sf::Vector2f& relativePosition);

    const sf::Vector2f& getCurrentPosition() const;
};

#endif