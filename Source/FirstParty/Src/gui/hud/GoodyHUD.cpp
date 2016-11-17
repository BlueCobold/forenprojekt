#include "GoodyHUD.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../../Input.hpp"

#include "../../model/Goody.hpp"

GoodyHUD::GoodyHUD(ResourceManager& resourceManager,
                   const std::string& iconKey,
                   const sf::IntRect& textureRect,
                   const sf::Vector2f& position,
                   const sf::Vector2f& offset) :
    HUDElement(position, offset),
    m_charges("", position, offset + sf::Vector2f(35, 44), 0, resourceManager.getBitmapFont("goodyFont"))
{
    m_icon.setTexture(*resourceManager.getTexture(iconKey));
    m_icon.setTextureRect(textureRect);

    m_startRect = textureRect;
    m_iconHeight = m_startRect.height;
}

void GoodyHUD::updated(const DrawParameter& params)
{
    m_icon.setPosition(getCurrentPosition());

    if(m_active)
        m_icon.setTextureRect(sf::IntRect(m_startRect.left, m_startRect.top + m_iconHeight * Active, m_startRect.width, m_startRect.height));
    else if(m_disabled)
    {
        if(m_selected)
            m_icon.setTextureRect(sf::IntRect(m_startRect.left, m_startRect.top + m_iconHeight * DisabledSelected, m_startRect.width, m_startRect.height));
        else
            m_icon.setTextureRect(sf::IntRect(m_startRect.left, m_startRect.top + m_iconHeight * Disabled, m_startRect.width, m_startRect.height));
    }
    else if(m_selected)
        m_icon.setTextureRect(sf::IntRect(m_startRect.left, m_startRect.top + m_iconHeight * Selected, m_startRect.width, m_startRect.height));
    else
        m_icon.setTextureRect(sf::IntRect(m_startRect.left, m_startRect.top + m_iconHeight * Deselected, m_startRect.width, m_startRect.height));
}

void GoodyHUD::doDraw(const DrawParameter& params)
{
    params.getTarget().draw(m_icon);
    m_charges.draw(params);
}

void GoodyHUD::updateState(const Goody& goody)
{
    int charges = goody.getCharges();

    m_selected = goody.isSelected();
    m_active = goody.isActive();
    m_disabled = (charges == 0);

    if(charges == -1)
        m_charges.setText(utility::toString("#"));
    else if(charges > 9)
        m_charges.setText(utility::toString("+"));
    else
        m_charges.setText(utility::toString(goody.getCharges()));
}
