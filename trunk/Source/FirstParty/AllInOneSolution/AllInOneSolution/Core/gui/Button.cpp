#include "Button.hpp"
#include "../Input.hpp"

#include <SFML/Window/Event.hpp>

Button::Button(int id, ButtonStyle style, const sf::Vector2f& position, const sf::Vector2f& offset) :
    m_position(position),
    m_id(id),
    m_offset(offset),
    m_style(style),
    m_playHoverSound(false)
{
    m_sprite = &m_style.idleStyle.sprite;
    m_label = &m_style.idleStyle.label;

    setPosition(position);

    m_size.x = m_style.idleStyle.sprite.getTextureRect().width;
    m_size.y = m_style.idleStyle.sprite.getTextureRect().height;
}

void Button::update(const sf::RenderWindow& screen)
{
    sf::IntRect buttonRect(static_cast<int>(m_position.x + m_offset.x + m_style.mouseRect.left - getSize().x / 2),
                           static_cast<int>(m_position.y + m_offset.y + m_style.mouseRect.top),
                           m_style.mouseRect.width,
                           m_style.mouseRect.height);

    sf::Vector2i mouseposition = sf::Mouse::getPosition(screen);
    if(buttonRect.contains(mouseposition))
    {
        if(!m_playHoverSound && m_style.hoverStyle.sound)
        {
            m_playHoverSound = true;
            m_style.hoverStyle.sound->play();
        }

        m_showToolTip = true;
        m_toolTip.setPosition(static_cast<const sf::Vector2f>(mouseposition));
        
        if(utility::Mouse.leftButtonPressed())
        {
            m_sprite = &m_style.pressedStyle.sprite;
            m_label = &m_style.pressedStyle.label;
            setPosition(m_position);
        } 
        else
        {
            m_sprite = &m_style.hoverStyle.sprite;
            m_label = &m_style.hoverStyle.label;
            setPosition(m_position);

            if(utility::Mouse.leftButtonReleased() && m_callback != nullptr)
                m_callback(*this);
        }
    }
    else
    {
        m_playHoverSound = false;
        m_sprite = &m_style.idleStyle.sprite;
        m_label = &m_style.idleStyle.label;
        setPosition(m_position);
        m_showToolTip = false;
    }
}

void Button::draw(const DrawParameter& params)
{
    params.getTarget().draw(*m_sprite);
    m_label->draw(params);
}

void Button::registerOnPressed(std::function<void (const Button& sender)> callback)
{
    m_callback = callback;
}

void Button::setPosition(const sf::Vector2f& position)
{
    m_position = position;
    sf::Vector2f half = sf::Vector2f(getSize().x / 2.f, 0);
    m_style.idleStyle.sprite.setPosition(m_position + m_offset + m_style.idleStyle.spriteOffset - half);
    m_style.idleStyle.label.setPosition(m_position + m_offset + m_style.idleStyle.textOffset);
    
    m_style.hoverStyle.sprite.setPosition(m_position + m_offset + m_style.hoverStyle.spriteOffset - half);
    m_style.hoverStyle.label.setPosition(m_position + m_offset + m_style.hoverStyle.textOffset);

    m_style.pressedStyle.sprite.setPosition(m_position + m_offset + m_style.pressedStyle.spriteOffset - half);
    m_style.pressedStyle.label.setPosition(m_position + m_offset + m_style.pressedStyle.textOffset);
}

const sf::Vector2i& Button::getSize() const
{
    return m_size;
}

int Button::getId() const
{
    return m_id;
}

void Button::setToolTip(const ToolTip& toolTip)
{
    m_toolTip = toolTip;
}

void Button::setToolTipText(const std::string& text)
{
    m_toolTip.setText(text);
}

void Button::drawAdditionalForeground(const DrawParameter& params)
{
    if(m_showToolTip)
        m_toolTip.draw(params);
}