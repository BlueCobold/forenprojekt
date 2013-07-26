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
    m_activeIcon.setTextureRect(sf::IntRect(textureRect.left, textureRect.top+goodyIconHeight, textureRect.width, textureRect.height));
    
    m_selectedIcon.setTexture(*resourceManager.getTexture(iconKey));
    m_selectedIcon.setTextureRect(sf::IntRect(textureRect.left, textureRect.top+goodyIconHeight*2, textureRect.width, textureRect.height));

    m_disabledIcon.setTexture(*resourceManager.getTexture(iconKey));
    m_disabledIcon.setTextureRect(sf::IntRect(textureRect.left, textureRect.top+goodyIconHeight*3, textureRect.width, textureRect.height));

    m_goodyState = Disabled;
}

void GoodyHUD::update(const DrawParameter& params)
{
    HUDElement::update(params);
    m_icon.setPosition(getPosition());
    m_activeIcon.setPosition(getPosition());
    m_selectedIcon.setPosition(getPosition());
    m_disabledIcon.setPosition(getPosition());
}

void GoodyHUD::draw(const DrawParameter& params)
{
    switch(m_goodyState)
    {
        case Deselected:
            params.getTarget().draw(m_icon);
            break;
        case Active:
            params.getTarget().draw(m_activeIcon);
            break;
        case Selected:
            params.getTarget().draw(m_selectedIcon);
            break;
        case Disabled:
            params.getTarget().draw(m_disabledIcon);
            break;
    }
}

void GoodyHUD::updateGoodyState(const Goody &goody)
{
    if(goody.isActive())
        m_goodyState = Active;
    else if(goody.getCharges() == 0)
        m_goodyState = Disabled;
    else if(goody.isSelected())
        m_goodyState = Selected;
    else if(!goody.isSelected())
        m_goodyState = Deselected;
}