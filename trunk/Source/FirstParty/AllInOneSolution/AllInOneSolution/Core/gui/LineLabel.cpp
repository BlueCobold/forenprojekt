#include "LineLabel.hpp"

LineLabel::LineLabel()
    : m_text(""),
    m_position(sf::Vector2f(0,0)),
    m_rotation(0),
    m_font(nullptr)
{
    setText("");
}

LineLabel::LineLabel(const std::string& text,
                    const sf::Vector2f& position,
                    const float rotation,
                    BitmapFont* font,
                    Alignment alignment)
    : m_text(text),
    m_position(position),
    m_rotation(rotation),
    m_font(font),
    m_alignment(alignment)
{
    setText(text);
}

void LineLabel::draw(const DrawParameter& params)
{
    for(auto it = begin(m_sprites); it != end(m_sprites); it++)  
        params.getTarget().draw(*it);
}

void LineLabel::setColor(const sf::Color &color)
{
	for(auto it = m_sprites.begin(); it != m_sprites.end(); ++it) it->setColor(color);
}

void LineLabel::setText(const std::string& text)
{
    if(text != m_text)
    {
        m_text = text;
        rebuild();
    }
}

std::string LineLabel::getText() const
{
    return m_text;
}

void LineLabel::setPosition(const sf::Vector2f position)
{
    if(position != m_position)
    {
        m_position = position;
        rebuild();
    }
}

void LineLabel::rebuild()
{
    m_sprites.clear();

    float shift = 0;
    if(m_alignment != Centered)
    {
        float width = 0;
        for(auto it = begin(m_text); it != end(m_text); it++)
            width += m_font->getSprite(*it).getTextureRect().width;
        if(m_alignment == Right)
            shift -= width;
        else
            shift -= width/2;
    }

    float xOffset = 0;
    for(auto it = begin(m_text); it != end(m_text); it++)
    {
        m_sprites.push_back(m_font->getSprite(*it));
        m_sprites.back().setPosition(shift + m_position.x + xOffset, m_position.y);
        m_sprites.back().setRotation(m_rotation);

        xOffset += m_font->getSprite(*it).getTextureRect().width;
    }
    m_width = xOffset;
}

void LineLabel::setPosition(const float x, const float y)
{
    m_position = sf::Vector2f(x, y);
}

void LineLabel::setAlignment(const Alignment alignment)
{
    if(m_alignment != alignment)
    {
        m_alignment = alignment;
        rebuild();
    }
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

void LineLabel::setBitmapFont(BitmapFont* font)
{
    m_font = font;
}

float LineLabel::getWidth() const
{
    return m_width;
}
