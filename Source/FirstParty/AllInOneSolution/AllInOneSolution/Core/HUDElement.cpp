#include "HUDElement.hpp"
#include "Utility.hpp"
#include "resources\ResourceManager.hpp"

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
    m_horizontalPercentage(horizontalPercentage)
{
}
