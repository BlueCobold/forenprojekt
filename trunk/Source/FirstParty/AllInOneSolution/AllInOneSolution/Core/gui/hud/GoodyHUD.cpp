#include "GoodyHUD.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../../Input.hpp"

GoodyHUD::GoodyHUD(ResourceManager& resourceManager,
                     const std::string& iconKey,
                     const sf::IntRect& textureRect,
                     const sf::Vector2f& position,
                     float hReference,
                     float vReference) :
    HUDElement(position, 0, hReference, vReference),
    m_selected(false)
{
    m_icon.setTexture(*resourceManager.getTexture(iconKey));
    m_icon.setTextureRect(textureRect);

    m_selectionSheen.setTexture(*resourceManager.getTexture("sheen"));
}

void GoodyHUD::update(const DrawParameter& params)
{
    HUDElement::update(params);
    m_icon.setPosition(getPosition());
    m_selectionSheen.setPosition(getPosition());
}

void GoodyHUD::draw(const DrawParameter& params)
{
    params.getTarget().draw(m_icon);
    if(m_selected)
        params.getTarget().draw(m_selectionSheen);
}

void GoodyHUD::setSelected(const bool selected)
{
  m_selected = selected;
}