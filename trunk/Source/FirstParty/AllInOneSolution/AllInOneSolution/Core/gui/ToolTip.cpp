#include "ToolTip.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

ToolTip::ToolTip(const std::string& text,
                 BitmapFont* font,
                 const sf::Vector2f& textOffset,
                 const sf::Vector2f& offset,
                 const sf::Sprite& leftTexture,
                 const sf::Sprite& centerTexture,
                 const sf::Sprite& rightTexture) :
    m_label(text, sf::Vector2f(0, 0), 0, font, LineLabel::Centered),
    m_leftTexture(leftTexture),
    m_centerTexture(centerTexture),
    m_rightTexture(rightTexture),
    m_offset(offset),
    m_position(sf::Vector2f(0, 0))
{
    m_label.setOffset(textOffset);
    m_scalefactor = m_label.getWidth() / m_centerTexture.getTextureRect().width;
    m_centerTexture.setScale(m_scalefactor, 1.f);
}
ToolTip::ToolTip() :
    m_label("", sf::Vector2f(0, 0), 0, nullptr, LineLabel::Centered),
    m_offset(sf::Vector2f(0, 0)),
    m_position(sf::Vector2f(0, 0)),
    m_scalefactor(1.f)
{
}
void ToolTip::draw(const DrawParameter& params)
{
    params.getTarget().draw(m_leftTexture);
    params.getTarget().draw(m_centerTexture);
    params.getTarget().draw(m_rightTexture);
    m_label.draw(params);
}

void ToolTip::setPosition(const sf::Vector2f& position)
{
    m_position = position;
    float length = m_label.getWidth() / 2;
    m_label.setPosition(m_position + m_offset);

    m_leftTexture.setPosition(m_position.x + m_offset.x - length - m_leftTexture.getTextureRect().width, 
                              m_position.y + m_offset.y);
    m_rightTexture.setPosition(m_position.x + m_offset.x + length, m_position.y + m_offset.y);
    m_centerTexture.setPosition(m_position.x + m_offset.x - length, m_position.y + m_offset.y);
}