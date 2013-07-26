#include "GoodyHUD.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../../Input.hpp"

#include "../../model/Goody.hpp"

GoodyHUD::GoodyHUD(ResourceManager& resourceManager,
                     const std::string& iconKey,
                     const sf::IntRect& textureRect,
                     const sf::Vector2f& position,
                     const int goodyIconHeight,
                     float hReference,
                     float vReference) :
    HUDElement(position, 0, hReference, vReference)
{
    m_icon.setTexture(*resourceManager.getTexture(iconKey));
    m_icon.setTextureRect(textureRect);

    m_startRect = textureRect;
    m_iconHeight = goodyIconHeight;
}

void GoodyHUD::update(const DrawParameter& params)
{
    HUDElement::update(params);
    m_icon.setPosition(getPosition());

    if(m_disabled)
    {
        if(m_selected)
            m_icon.setTextureRect(sf::IntRect(m_startRect.left, m_startRect.top + m_iconHeight * 4, m_startRect.width, m_startRect.height));
        else
            m_icon.setTextureRect(sf::IntRect(m_startRect.left, m_startRect.top + m_iconHeight * 3, m_startRect.width, m_startRect.height));
    }
    else if(m_active)
        m_icon.setTextureRect(sf::IntRect(m_startRect.left, m_startRect.top + m_iconHeight, m_startRect.width, m_startRect.height));
    else if(m_selected)
        m_icon.setTextureRect(sf::IntRect(m_startRect.left, m_startRect.top + m_iconHeight * 2, m_startRect.width, m_startRect.height));
    else
        m_icon.setTextureRect(m_startRect);
}

void GoodyHUD::draw(const DrawParameter& params)
{
    params.getTarget().draw(m_icon);
}

void GoodyHUD::updateGoodyState(const Goody &goody)
{
    m_selected = goody.isSelected();
    m_active = goody.isActive();
    m_disabled = (goody.getCharges() == 0);
}