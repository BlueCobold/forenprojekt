#include "ScreenLocation.hpp"

const float ScreenLocation::Left = 0.0f;
const float ScreenLocation::Right = 1.0f;
const float ScreenLocation::Center = 0.5f;
const float ScreenLocation::Top = 0.0f;
const float ScreenLocation::Bottom = 1.0f;

ScreenLocation::ScreenLocation(const sf::Vector2f& relativePosition,
                               const sf::Vector2f& offset,
                               const sf::Vector2f& screenSize) :
    m_relativePosition(relativePosition),
    m_offset(offset),
    m_screenSize(screenSize)
{
    calculatePosition();
}

void ScreenLocation::calculatePosition()
{
    m_currentPosition = sf::Vector2f(m_screenSize.x * m_relativePosition.x,
                                     m_screenSize.y * m_relativePosition.y) + m_offset;
}

void ScreenLocation::setScreenSize(const sf::Vector2f& screenSize)
{
    if(m_screenSize != screenSize)
    {
        m_screenSize = screenSize;
        calculatePosition();
    }
}

void ScreenLocation::setOffset(const sf::Vector2f& offset)
{
    if(m_offset != offset)
    {
        m_offset = offset;
        calculatePosition();
    }
}

void ScreenLocation::setPosition(const sf::Vector2f& relativePosition)
{
    if(m_relativePosition != relativePosition)
    {
        m_relativePosition = relativePosition;
        calculatePosition();
    }
}

const sf::Vector2f& ScreenLocation::getPosition() const
{
    return m_relativePosition;
}

const sf::Vector2f& ScreenLocation::getCurrentPosition() const
{
    return m_currentPosition;
}

const sf::Vector2f& ScreenLocation::getOffset() const
{
    return m_offset;
}