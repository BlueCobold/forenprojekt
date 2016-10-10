#include "LabelHUD.hpp"
#include "../../Utility.hpp"
#include "../../resources/ResourceManager.hpp"

LabelHUD::LabelHUD(const sf::Vector2f& position,
                   const sf::Vector2f& offset,
                   const BitmapFont* bitmapFont,
                   const std::string& text,
                   const LineLabel::Alignment alignment) :
    HUDElement(position, offset),
    m_label(text, position, offset, 0, bitmapFont, "", alignment)
{
}

void LabelHUD::update(const DrawParameter& params)
{
    HUDElement::update(params);
    m_label.updateLayout(static_cast<sf::Vector2f>(params.getTarget().getSize()));
}

void LabelHUD::doDraw(const DrawParameter& params)
{    
    m_label.draw(params);
}

void LabelHUD::setText(const std::string& text)
{
    m_label.setText(text);
}
void LabelHUD::setPosition(const sf::Vector2f& position, const sf::Vector2f& offset)
{
    HUDElement::setPosition(position, offset);
    m_label.setPosition(position);
    m_label.setOffset(offset);
}