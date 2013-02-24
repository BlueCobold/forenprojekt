#include "LineLabel.hpp"

LineLabel::LineLabel()
    : m_text(""),
    m_font(nullptr)
{
    setText("");
    m_position = sf::Vector2f(0.f,0.f);
    m_rotation = 0.f;
}

LineLabel::LineLabel(const std::string& text, const sf::Vector2f& position, const float rotation, BitmapFont* font)
    : m_text(text),
    m_position(position),
    m_rotation(rotation),
    m_font(font)
{
    setText(text);
    m_aligend = LeftAligend;
}

LineLabel::LineLabel(const std::string& text, const float x, const float y, float rotation, BitmapFont* font)
    : m_text(text),
    m_position(sf::Vector2f(x,y)),
    m_rotation(rotation),
    m_font(font)
{
    setText(text);
    m_aligend = LeftAligend;
}
void LineLabel::draw(const DrawParameter& params)
{
    sf::Sprite sprite(m_texture.getTexture());
    sprite.setPosition(m_position);
    sprite.setRotation(m_rotation);

    switch(m_aligend)
    {
        case LeftAligend:
            sprite.setOrigin(0.f,0.f);
        break;

        case CenterAligend:
            sprite.setOrigin(m_texture.getSize().x / 2.f, 0.f);
        break;

        case RightAligend:
            sprite.setOrigin(static_cast<float>(m_texture.getSize().x), 0.f);
        break;

        default:
            sprite.setOrigin(0.f,0.f);
        break;
    }

    params.getTarget().draw(sprite);
}

void LineLabel::update()
{
    
}
void LineLabel::setText(const std::string& text)
{
    
    if(text != m_text)
    {
        std::vector<sf::Sprite> sprites;

        float xOffset = 0.f;

        sprites.clear();
  
        for(auto it = begin(text); it != end(text); it++)
        {
            sprites.push_back(m_font->getSprite(*it));
            sprites.back().setPosition(0 + xOffset, 0);

            xOffset += m_font->getSprite(*it).getTextureRect().width;
        }

        m_texture.create(static_cast<unsigned int>(xOffset), m_font->getFontSize());

        m_texture.clear(sf::Color(0,0,0,0));

        for(auto it = begin(sprites); it != end(sprites); it++)  
            m_texture.draw(*it);

        m_texture.display();

        m_text = text;
    }
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
    m_font = &font;
}

int LineLabel::getLength()
{
    return m_texture.getSize().x;
}

void LineLabel::setAligend(Aligend value)
{
    m_aligend = value;
}