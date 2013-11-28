#include "Button.hpp"
#include "../Input.hpp"

#include <SFML/Window/Event.hpp>

Button::Button(int id, ButtonStyle style, const sf::Vector2f& position, const sf::Vector2f& offset) :
    MenuElement(id, MenuElementType::Button, position, offset),
    m_style(style),
    m_playHoverSound(false)
{
    m_sprite = &m_style.idleStyle.sprite;
    m_label = &m_style.idleStyle.label;

    onPositionChanged();

    m_size.x = m_style.idleStyle.sprite.getTextureRect().width;
    m_size.y = m_style.idleStyle.sprite.getTextureRect().height;
}

void Button::update(const sf::RenderWindow& screen, const sf::Vector2i& mouseOffset)
{
    auto position = getPosition();
    auto offset = getOffset();
    sf::IntRect buttonRect(static_cast<int>(position.x + offset.x + m_style.mouseRect.left - getSize().x / 2),
                           static_cast<int>(position.y + offset.y + m_style.mouseRect.top),
                           m_style.mouseRect.width,
                           m_style.mouseRect.height);

    sf::Vector2i mouseposition = sf::Mouse::getPosition(screen);
    if(buttonRect.contains(mouseposition + mouseOffset) && isVisible())
    {
        if(!m_playHoverSound && m_style.hoverStyle.sound)
        {
            m_playHoverSound = true;
            m_style.hoverStyle.sound->play();
        }

        m_showToolTip = true;
        m_toolTip.setPosition(static_cast<const sf::Vector2f>(mouseposition), screen);
        
        if(utility::Mouse.leftButtonPressed())
        {
            m_sprite = &m_style.pressedStyle.sprite;
            m_label = &m_style.pressedStyle.label;
            onPositionChanged();

            if(!m_playPressedSound && m_style.pressedStyle.sound)
            {
                m_playPressedSound = true;
                m_style.pressedStyle.sound->play();
            }
        } 
        else
        {
            m_playPressedSound = false;
            m_sprite = &m_style.hoverStyle.sprite;
            m_label = &m_style.hoverStyle.label;
            onPositionChanged();

            if(utility::Mouse.leftButtonReleased() && m_callback != nullptr)
                m_callback(*this);
        }
    }
    else
    {
        m_playHoverSound = false;
        m_playPressedSound = false;
        m_sprite = &m_style.idleStyle.sprite;
        m_label = &m_style.idleStyle.label;
        onPositionChanged();
        m_showToolTip = false;
    }
}

void Button::draw(const DrawParameter& params)
{
    if(!isVisible())
        return;

    params.getTarget().draw(*m_sprite);
    m_label->draw(params);
}

void Button::registerOnPressed(std::function<void (const Button& sender)> callback)
{
    m_callback = callback;
}

void Button::onPositionChanged()
{
    auto position = getPosition();
    auto offset = getOffset();
    sf::Vector2f half = sf::Vector2f(getSize().x / 2.f, 0);
    m_style.idleStyle.sprite.setPosition(position + offset + m_style.idleStyle.spriteOffset - half);
    m_style.idleStyle.label.setPosition(position + offset + m_style.idleStyle.textOffset);
    
    m_style.hoverStyle.sprite.setPosition(position + offset + m_style.hoverStyle.spriteOffset - half);
    m_style.hoverStyle.label.setPosition(position + offset + m_style.hoverStyle.textOffset);

    m_style.pressedStyle.sprite.setPosition(position + offset + m_style.pressedStyle.spriteOffset - half);
    m_style.pressedStyle.label.setPosition(position + offset + m_style.pressedStyle.textOffset);
}

const sf::Vector2i& Button::getSize() const
{
    return m_size;
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
    if(m_showToolTip && isVisible())
        m_toolTip.draw(params);
}

void Button::changeIdleSprite(const sf::Sprite& sprite)
{
    sf::Sprite s(sprite);
    s.setPosition(m_style.idleStyle.sprite.getPosition());
    m_style.idleStyle.sprite = s;
}

void Button::changeHoverSprite(const sf::Sprite& sprite)
{
    sf::Sprite s(sprite);
    s.setPosition(m_style.hoverStyle.sprite.getPosition());
    m_style.hoverStyle.sprite = s;
}

void Button::changePressedSprite(const sf::Sprite& sprite)
{
    sf::Sprite s(sprite);
    s.setPosition(m_style.pressedStyle.sprite.getPosition());
    m_style.pressedStyle.sprite = s;
}
