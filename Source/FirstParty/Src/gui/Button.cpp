#include "Button.hpp"
#include "../Input.hpp"

#include <SFML/Window/Event.hpp>

Button::Button(int id, ButtonStyle style, const sf::Vector2f& position, const sf::Vector2f& offset, bool triggers) :
    MenuElement(id, MenuElementType::Button, position, offset),
    m_style(std::move(style)),
    m_showToolTip(false),
    m_isTriggering(triggers),
    m_playHoverSound(false),
    m_playPressedSound(false)
{
    m_sprite = &m_style.idleStyle.sprite;
    m_label = &m_style.idleStyle.label;
    m_animation = m_style.idleStyle.animation.get();
    m_resetAnimation = m_style.idleStyle.resetOnExit;

    m_size.x = m_style.idleStyle.sprite.getTextureRect().width;
    m_size.y = m_style.idleStyle.sprite.getTextureRect().height;
}

std::unique_ptr<MenuElement> Button::clone() const
{
    auto clone = std::unique_ptr<Button>(new Button(getId(), ButtonStyle(m_style), getPosition(), getOffset(), m_isTriggering));
    clone->setVisibleWhenId(getVisibleWhenId());
    clone->m_toolTip = m_toolTip;
    return std::move(clone);
}

void Button::update(const sf::RenderWindow& screen, const float time, const sf::Vector2i& mouseOffset)
{
    updateLayout(static_cast<sf::Vector2f>(screen.getSize()));

    auto currentPosition = getCurrentPosition();
    sf::IntRect buttonRect(static_cast<int>(currentPosition.x + m_style.mouseRect.left - getSize().x / 2),
                           static_cast<int>(currentPosition.y + m_style.mouseRect.top),
                           m_style.mouseRect.width,
                           m_style.mouseRect.height);
    auto mousePosition = getCursorPosition(screen);

    if(buttonRect.contains(mousePosition + mouseOffset) && isVisible())
    {
        if(!m_playHoverSound && m_style.hoverStyle.sound)
        {
            m_playHoverSound = true;
            m_style.hoverStyle.sound->play();
        }

        m_showToolTip = true;
        m_toolTip.setPosition(static_cast<const sf::Vector2f>(mousePosition), screen);

        if(utility::Mouse.leftButtonPressed())
        {
            setStyle(m_style.pressedStyle, time);

            if(!m_playPressedSound && m_style.pressedStyle.sound)
            {
                m_playPressedSound = true;
                m_style.pressedStyle.sound->play();
            }
        }
        else
        {
            setStyle(m_style.hoverStyle, time);
            m_playPressedSound = false;

            if(m_isTriggering && utility::Mouse.leftButtonReleased() && m_callback != nullptr)
                m_callback(*this);
        }
    }
    else
    {
        m_playHoverSound = false;
        m_playPressedSound = false;
        setStyle(m_style.idleStyle, time);
        m_showToolTip = false;
    }

    if(m_animation != nullptr)
        m_animation->update(screen, time, mouseOffset);
}

void Button::setStyle(ButtonStateStyle& style, float time)
{
    m_sprite = &style.sprite;
    m_label = &style.label;
    
    if(m_animation != style.animation.get())
    {
        if(m_resetAnimation)
        {
            m_style.idleStyle.animation->restartAt(time);
            m_style.hoverStyle.animation->restartAt(time);
            m_style.pressedStyle.animation->restartAt(time);
        }
        m_animation = style.animation.get();
        m_resetAnimation = style.resetOnExit;
    }
}

void Button::draw(const DrawParameter& params)
{
    if(!isVisible())
        return;

    if(m_animation != nullptr)
        m_animation->draw(params);
    else
    {
        params.getTarget().draw(*m_sprite);
        m_label->draw(params);
    }
}

void Button::registerOnPressed(std::function<void (const Button& sender)> callback)
{
    m_callback = callback;
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

    m_showToolTip = false;
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

void Button::updateLayout(const sf::Vector2f& screenSize)
{
    MenuElement::updateLayout(screenSize);
    m_style.idleStyle.label.updateLayout(screenSize);
    m_style.hoverStyle.label.updateLayout(screenSize);
    m_style.pressedStyle.label.updateLayout(screenSize);

    sf::Vector2f half = sf::Vector2f(getSize().x / 2.f, 0);
    auto currentPosition = getCurrentPosition();

    m_style.idleStyle.sprite.setPosition(currentPosition + m_style.idleStyle.spriteOffset - half);
    m_style.hoverStyle.sprite.setPosition(currentPosition + m_style.hoverStyle.spriteOffset - half);
    m_style.pressedStyle.sprite.setPosition(currentPosition + m_style.pressedStyle.spriteOffset - half);
}