#include "HUDElement.hpp"
#include "Utility.hpp"
#include "resources\ResourceManager.hpp"

HUDElement::HUDElement(ResourceManager& resourceManager, const sf::Vector2f& position, const float rotation, std::string bitmapFont,
HorizontalReference hReference, VerticalReference vReference, std::string text) :
m_resourceManager(resourceManager),
LineLabel(text,sf::Vector2f(0.f,0.f),rotation,nullptr),
m_screenPosition(position),
m_verticalReference(vReference),
m_horizontalReference(hReference)
{
    m_font = m_resourceManager.getBitmapFont(bitmapFont);
}

void HUDElement::update(const DrawParameter& params)
{
    sf::Vector2f screenSize = params.getTarget().getView().getSize();
    sf::Vector2f screenCenter = params.getTarget().getView().getCenter();
    sf::Vector2f position;

    switch(m_horizontalReference)
    {
        case HR_Left:
            position.x = screenCenter.x - screenSize.x / 2 + m_screenPosition.x;
        break;
        case HR_Center:
            position.x = screenCenter.x;
        break;
        case HR_Right:
            position.x = screenCenter.x + screenSize.x / 2 - m_screenPosition.x;
        break;
        default:
            position.x = m_screenPosition.x;
        break;
    }

    switch(m_verticalReference)
    {
        case VR_Top:
            position.y = screenCenter.y - screenSize.y / 2 + m_screenPosition.y;
        break;
        case VR_Center:
            position.y = screenCenter.y;
        break;
        case VR_Bottom:
            position.y = screenCenter.y + screenSize.y / 2 - m_screenPosition.y;
        break;
        default:
            position.y = m_screenPosition.y;
        break;
    }

    setPosition(position);
}

void HUDElement::draw(const DrawParameter& params)
{    
    LineLabel::draw(params);
}

void HUDElement::setScreenPosition(sf::Vector2f position, HorizontalReference hReference , VerticalReference vReference)
{
    m_screenPosition = position;
    m_horizontalReference = hReference;
    m_verticalReference = vReference;
}

void HUDElement::setText(const std::string& text)
{
    LineLabel::setText(text);
}

void HUDElement::setScreenPositionPercent(HorizontalReference hReference, float hRefPercent, VerticalReference vReference, float vRefPercent, sf::Vector2f screenSize)
{
    sf::Vector2f screenCenter = sf::Vector2f(screenSize.x / 2.f, screenSize.y / 2.f);
    switch(m_horizontalReference)
    {
        case HR_Left:
            m_screenPosition.x = 0.f + screenSize.x * hRefPercent;
        break;
        case HR_Center:
            m_screenPosition.x = screenCenter.x + screenSize.x / 2.f * hRefPercent;
        break;
        case HR_Right:
            m_screenPosition.x = screenSize.x + screenSize.x * hRefPercent;
        break;
        default:
            m_screenPosition.x = 0.f + screenSize.x * hRefPercent;
        break;
    }

    switch(m_verticalReference)
    {
        case VR_Top:
            m_screenPosition.y = 0.f + screenSize.y * vRefPercent;
        break;
        case VR_Center:
            m_screenPosition.y = screenCenter.y + screenSize.y / 2.f * vRefPercent;
        break;
        case VR_Bottom:
            m_screenPosition.y = screenSize.y + screenSize.y * vRefPercent;
        break;
        default:
            m_screenPosition.y = 0.f + screenSize.y * vRefPercent;
        break;
    }
}