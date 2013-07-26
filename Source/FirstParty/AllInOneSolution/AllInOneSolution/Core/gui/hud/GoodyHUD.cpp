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

    m_activeIcon.setTexture(*resourceManager.getTexture(iconKey));
    m_activeIcon.setTextureRect(sf::IntRect(textureRect.left, textureRect.top + goodyIconHeight, textureRect.width, textureRect.height));
    
    m_selectedIcon.setTexture(*resourceManager.getTexture(iconKey));
    m_selectedIcon.setTextureRect(sf::IntRect(textureRect.left, textureRect.top + goodyIconHeight * 2, textureRect.width, textureRect.height));

    m_disabledUnSelIcon.setTexture(*resourceManager.getTexture(iconKey));
    m_disabledUnSelIcon.setTextureRect(sf::IntRect(textureRect.left, textureRect.top + goodyIconHeight * 3, textureRect.width, textureRect.height));

    m_disabledSelIcon.setTexture(*resourceManager.getTexture(iconKey));
    m_disabledSelIcon.setTextureRect(sf::IntRect(textureRect.left, textureRect.top + goodyIconHeight * 4, textureRect.width, textureRect.height));

    m_goodyState = Disabled;
}

void GoodyHUD::update(const DrawParameter& params)
{
    HUDElement::update(params);
    m_icon.setPosition(getPosition());
    m_activeIcon.setPosition(getPosition());
    m_selectedIcon.setPosition(getPosition());
    m_disabledSelIcon.setPosition(getPosition());
    m_disabledUnSelIcon.setPosition(getPosition());
}

void GoodyHUD::draw(const DrawParameter& params)
{
    if(m_disabled)
    {
        if(m_selected)
            params.getTarget().draw(m_disabledSelIcon);    
        else
            params.getTarget().draw(m_disabledUnSelIcon);
    }
    else if(m_active)
        params.getTarget().draw(m_activeIcon);
    else if(m_selected)
        params.getTarget().draw(m_selectedIcon);
    else
        params.getTarget().draw(m_icon);
}

void GoodyHUD::updateGoodyState(const Goody &goody)
{
    m_selected = goody.isSelected();
    m_active = goody.isActive();
    m_disabled = (goody.getCharges() == 0);
}