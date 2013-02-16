#include "LineLabel.hpp"

LineLabel::LineLabel()
    : m_text(""),
    m_position(sf::Vector2f(0,0)),
    m_rotation(0),
    m_font(BitmapFont())
{
}

LineLabel::LineLabel(const std::string& text, const sf::Vector2f& position, const float rotation, BitmapFont& font)
    : m_text(text),
    m_position(position),
    m_rotation(rotation),
    m_font(font)
{
}

LineLabel::LineLabel(const std::string& text, const float x, const float y, float rotation, BitmapFont& font)
    : m_text(text),
    m_position(sf::Vector2f(x,y)),
    m_rotation(rotation),
    m_font(font)
{
}

void LineLabel::draw(const DrawParameter& params)
{
    float xOffset = 0;
    for(auto it = m_text.begin(); it != m_text.end(); it++)
    {
        sf::Sprite tmp = m_font.getSprite(*it);
        tmp.setPosition(m_position.x + xOffset, m_position.y);
        tmp.setRotation(m_rotation);

        params.getTarget().draw(tmp);

        xOffset += m_font.getSprite(*it).getTextureRect().width;
    }
}

void LineLabel::setText(const std::string& text)
{
    m_text = text;
}

std::string LineLabel::getText() const
{
    return m_text;
}

void LineLabel::setPosition(const sf::Vector2f position)
{
    m_position = position;
}

void LineLabel::setPosition(const float x, const float y)
{
    m_position = sf::Vector2f(x, y);
}

sf::Vector2f LineLabel::getPosition() const
{
    return m_position;
}

void LineLabel::setRotation(const float rotation)
{
    m_rotation = rotation;
}

float LineLabel::getRotation() const
{
    return m_rotation;
}

void LineLabel::setBitmapFont(BitmapFont& font)
{
    m_font = font;
}
