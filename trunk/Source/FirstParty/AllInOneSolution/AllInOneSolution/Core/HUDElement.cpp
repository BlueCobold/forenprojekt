#include "HUDElement.hpp"
#include "Utility.hpp"
#include "resources\ResourceManager.hpp"

HUDElement::HUDElement(ResourceManager& resourceManager, const sf::Vector2f& position, const float rotation, std::string bitmapFont,
HorizontalReference hReference, VerticalReference vReference, std::string text) :
m_resourceManager(resourceManager),
m_label(text,sf::Vector2f(0.f,0.f),rotation,nullptr),
m_bitmapfont(nullptr),
m_position(position),
m_verticalReference(vReference),
m_horizontalReference(hReference)
{
    m_bitmapfont = m_resourceManager.getBitmapFont(bitmapFont);
    m_label.setBitmapFont(*m_bitmapfont);
}

void HUDElement::update(const DrawParameter& params)
{
    sf::Vector2f screenSize = params.getTarget().getView().getSize();
    sf::Vector2f screenCenter = params.getTarget().getView().getCenter();
    sf::Vector2f position;

    switch(m_horizontalReference)
    {
        case HUDElement::HR_Left:
            position.x = screenCenter.x - screenSize.x / 2 + m_position.x;
        break;
        case HUDElement::HR_Center:
            position.x = screenCenter.x;
        break;
        case HUDElement::HR_Right:
            position.x = screenCenter.x + screenSize.x / 2 - m_position.x;
        break;
        default:
            position.x = m_position.x;
        break;
    }

    switch(m_verticalReference)
    {
        case HUDElement::VR_Top:
            position.y = screenCenter.y - screenSize.y / 2 + m_position.y;
        break;
        case HUDElement::VR_Center:
            position.y = screenCenter.y;
        break;
        case HUDElement::VR_Bottom:
            position.y = screenCenter.y + screenSize.y / 2 - m_position.y;
        break;
        default:
            position.y = m_position.y;
        break;
    }

    m_label.setPosition(position);
}

void HUDElement::draw(const DrawParameter& params)
{    
    m_label.draw(params);
}

void HUDElement::setPosition(sf::Vector2f position, HorizontalReference hReference , VerticalReference vReference)
{
    m_position = position;
    m_horizontalReference = hReference;
    m_verticalReference = vReference;
}

void HUDElement::setText(std::string text)
{
    m_label.setText(text);
}