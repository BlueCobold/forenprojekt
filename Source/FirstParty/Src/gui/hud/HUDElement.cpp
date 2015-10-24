#include "HUDElement.hpp"
#include "../../Utility.hpp"
#include "../../resources/ResourceManager.hpp"

HUDElement::HUDElement(const sf::Vector2f& position,
                       const sf::Vector2f& offset) :
    m_screenLocation(position, offset)
{
}

void HUDElement::update(const DrawParameter& params)
{
    m_screenLocation.setScreenSize(static_cast<sf::Vector2f>(params.getTarget().getSize()));
}

const sf::Vector2f HUDElement::getCurrentPosition() const
{
    return m_screenLocation.getCurrentPosition();
}

void HUDElement::setPosition(const sf::Vector2f& position, const sf::Vector2f& offset)
{
    m_screenLocation.setPosition(position);
    m_screenLocation.setOffset(offset);
}
