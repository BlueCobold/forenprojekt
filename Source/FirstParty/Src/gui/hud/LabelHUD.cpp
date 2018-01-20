#include "LabelHUD.hpp"
#include "../../Utility.hpp"
#include "../../resources/ResourceManager.hpp"

LabelHUD::LabelHUD(const sf::Vector2f& position,
                   const sf::Vector2f& offset,
                   const BitmapFont* bitmapFont,
                   const std::string& text,
                   const LineLabel::Alignment alignment) :
    HUDElement(position, offset),
    m_label(text, ScreenLocation(position, offset), ScreenScale(), 0, bitmapFont, alignment)
{
}

void LabelHUD::updated(const DrawParameter& params)
{
    m_label.updateLayout(sf::Vector2f(params.getTarget().getSize()));
}

void LabelHUD::doDraw(const DrawParameter& params)
{    
    m_label.draw(params);
}

void LabelHUD::setText(const std::string& text)
{
    m_label.setText(text);
}

void LabelHUD::onPositionSet(const sf::Vector2f& position, const sf::Vector2f& offset)
{
    m_label.setPosition(position);
    m_label.setOffset(offset);
}

float LabelHUD::getLabelWidth() const
{
    return m_label.getWidth();
}

void LabelHUD::setAlignment(const LineLabel::Alignment alignment)
{
    m_label.setAlignment(alignment);
}
