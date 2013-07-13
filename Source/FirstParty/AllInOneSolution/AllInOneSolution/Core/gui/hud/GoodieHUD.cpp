#include "GoodieHUD.hpp"
#include "../../resources/ResourceManager.hpp"

GoodieHUD::GoodieHUD(ResourceManager& resourceManager,
                     const std::string& iconKey,
                     const sf::IntRect& textureRect,
                     const sf::Vector2f& position,
                     float hReference,
                     float vReference) :
    HUDElement(position, 0, hReference, vReference)
{
    m_icon.setTexture(*resourceManager.getTexture(iconKey));
    m_icon.setTextureRect(textureRect);
}

void GoodieHUD::update(const DrawParameter& params)
{
    HUDElement::update(params);
    m_icon.setPosition(getPosition());
}

void GoodieHUD::draw(const DrawParameter& params)
{
    params.getTarget().draw(m_icon);
}