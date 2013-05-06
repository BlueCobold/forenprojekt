#include "Button.hpp"
#include "../Input.hpp"

#include <SFML/Window/Event.hpp>

Button::Button(const sf::Vector2f& position,
               const std::string& text,
               BitmapFont* font,
               const sf::Sprite& buttonIdlePicture,
               const int id,
               const sf::Vector2f& offset,
               const sf::Vector2f& textOffset) :
    m_position(position),
    m_buttonLabel(text, sf::Vector2f(0, 0), 0, font, LineLabel::Centered),
    m_id(id),
    m_offset(offset),
    m_textOffset(textOffset)
{
    m_idleData.m_bound = true;
    m_idleData.m_font = font;
    m_idleData.m_sprite = buttonIdlePicture;
    m_idleData.m_spriteOffset = sf::Vector2f(0, 0);
    m_idleData.m_fontOffset = sf::Vector2f(0, 0);

    m_hoverData.m_bound = m_pressedData.m_bound = false;
    m_buttonSprite = buttonIdlePicture;
    m_buttonSprite.setPosition(m_position + offset);

    m_textOffset.x += buttonIdlePicture.getTextureRect().width / 2;
    m_textOffset.y += buttonIdlePicture.getTextureRect().height / 2 - font->getFontSize() / 2;

    m_buttonLabel.setPosition(position + offset + m_textOffset);

    m_size.x = m_buttonSprite.getTextureRect().width;
    m_size.y = m_buttonSprite.getTextureRect().height;
}

void Button::update(const sf::RenderWindow& screen)
{
    sf::FloatRect buttonRect(m_position.x + m_offset.x + m_idleData.m_spriteOffset.x + 2,
                             m_position.y + m_offset.y + m_idleData.m_spriteOffset.y + 2,
                             static_cast<float>(m_idleData.m_sprite.getTextureRect().width - 2),
                             static_cast<float>(m_idleData.m_sprite.getTextureRect().height - 2));

    if(buttonRect.contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(screen))))
    {
        if(utility::Mouse.leftButtonPressed())
        {
            if (m_pressedData.m_bound)
            {
                m_buttonSprite = m_pressedData.m_sprite;
                m_buttonLabel.setOffset(m_pressedData.m_fontOffset);
                m_buttonLabel.setBitmapFont(m_pressedData.m_font);
            }
        } 
        else
        {
            if(m_hoverData.m_bound)
            {
                m_buttonSprite = m_hoverData.m_sprite;
                m_buttonLabel.setOffset(m_hoverData.m_fontOffset);
                m_buttonLabel.setBitmapFont(m_hoverData.m_font);
            }
            else
            {
                m_buttonSprite = m_idleData.m_sprite;
                m_buttonLabel.setOffset(m_idleData.m_fontOffset);
                m_buttonLabel.setBitmapFont(m_idleData.m_font);
            }
            if(utility::Mouse.leftButtonReleased() && m_callback != nullptr)
                m_callback(*this);
        }

    }
    else
    {
        m_buttonSprite = m_idleData.m_sprite;
        m_buttonLabel.setOffset(m_idleData.m_fontOffset);
        m_buttonLabel.setBitmapFont(m_idleData.m_font);
    }
}

void Button::draw(const DrawParameter& params)
{
    params.getTarget().draw(m_buttonSprite);
    m_buttonLabel.draw(params);
}

void Button::bindHover(const sf::Sprite& sprite,
                       BitmapFont* font,
                       const sf::Vector2f& spriteOffset,
                       const sf::Vector2f& fontOffset)
{
    m_hoverData.m_bound = true;
    m_hoverData.m_sprite = sprite;
    m_hoverData.m_sprite.setPosition(m_position + m_offset + spriteOffset);
    m_hoverData.m_spriteOffset = spriteOffset;
    m_hoverData.m_font = font;
    m_hoverData.m_fontOffset = fontOffset;
}

void Button::bindPressed(const sf::Sprite& sprite,
                         BitmapFont* font,
                         const sf::Vector2f& spriteOffset,
                         const sf::Vector2f& fontOffset)
{
    m_pressedData.m_bound = true;
    m_pressedData.m_sprite = sprite;
    m_pressedData.m_sprite.setPosition(m_position + m_offset + spriteOffset);
    m_pressedData.m_spriteOffset = spriteOffset;
    m_pressedData.m_font = font;
    m_pressedData.m_fontOffset = fontOffset;
}

void Button::registerOnPressed(std::function<void (const Button& sender)> callback)
{
    m_callback = callback;
}

void Button::setPosition(const sf::Vector2f& position)
{
    m_position = position;
    m_idleData.m_sprite.setPosition(m_position + m_offset + m_idleData.m_spriteOffset);
    m_buttonLabel.setPosition(m_position + m_offset + m_textOffset);

    if(m_hoverData.m_bound)
        m_hoverData.m_sprite.setPosition(m_position + m_offset + m_hoverData.m_spriteOffset);

    if(m_pressedData.m_bound)
        m_pressedData.m_sprite.setPosition(m_position + m_offset + m_pressedData.m_spriteOffset);
}

const sf::Vector2i& Button::getSize() const
{
    return m_size;
}

int Button::getId() const
{
    return m_id;
}
