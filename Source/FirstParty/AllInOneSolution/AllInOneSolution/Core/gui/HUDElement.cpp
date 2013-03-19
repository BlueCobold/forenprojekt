#include "HUDElement.hpp"
#include "../Utility.hpp"
#include "../resources\ResourceManager.hpp"

const float HUDElement::Left = 0.0f;
const float HUDElement::Right = 1.0f;
const float HUDElement::Center= 0.5f;
const float HUDElement::Top = 0.0f;
const float HUDElement::Middle = 0.5f;
const float HUDElement::Bottom = 1.0f;

HUDElement::HUDElement(const sf::Vector2f& position, 
                       const float rotation,
                       const float horizontalPercentage, 
                       const float verticalPercentage) :
    m_position(position),
    m_verticalPercentage(verticalPercentage),
    m_horizontalPercentage(horizontalPercentage),
    m_currentPosition(sf::Vector2f(0, 0))
{
}

void HUDElement::update(const DrawParameter& params)
{
    sf::Vector2f screenSize = params.getTarget().getView().getSize();
    sf::Vector2f screenCenter = params.getTarget().getView().getCenter();
    
    sf::Vector2f topLeftViewStart = screenCenter - screenSize * 0.5f;
    sf::Vector2f bottomRightViewEnd = screenCenter + screenSize * 0.5f;
    sf::Vector2f delta = bottomRightViewEnd - topLeftViewStart;

    m_currentPosition.x =  topLeftViewStart.x + delta.x * m_horizontalPercentage + m_position.x;
    m_currentPosition.y =  topLeftViewStart.y + delta.y * m_verticalPercentage + m_position.y;
}

const sf::Vector2f HUDElement::getPosition() const
{
    return m_currentPosition;
}

void HUDElement::setPosition(const sf::Vector2f& position, const float horizontalPercentage, const float verticalPercentage)
{
    m_position = position;
    m_verticalPercentage = verticalPercentage;
    m_horizontalPercentage = horizontalPercentage;
}