#include "Button.hpp"
#include "../Input.hpp"

#include <SFML/Window/Event.hpp>

Button::Button(int id, ButtonStyle style,
               const ScreenLocation& position,
               const ScreenScale& scale,
               bool triggers) :
    MenuElement(id, MenuElementType::Button, position),
    m_style(std::move(style)),
    m_showToolTip(false),
    m_isTriggering(triggers),
    m_playHoverSound(false),
    m_playPressedSound(false),
    m_scale(scale)
{
    m_currentStyle = &m_style.idleStyle;

    if(m_style.idleStyle.animation != nullptr)
        m_style.idleStyle.animation->setPosition(ScreenLocation(position).addOffset(m_style.idleStyle.spriteOffset));

    if(m_style.hoverStyle.animation != nullptr)
        m_style.hoverStyle.animation->setPosition(ScreenLocation(position).addOffset(m_style.hoverStyle.spriteOffset));

    if(m_style.pressedStyle.animation != nullptr)
        m_style.pressedStyle.animation->setPosition(ScreenLocation(position).addOffset(m_style.pressedStyle.spriteOffset));
}

std::unique_ptr<MenuElement> Button::onClone() const
{
    auto clone = std::unique_ptr<Button>(new Button(getId(), m_style, getPosition(), m_scale, m_isTriggering));
    clone->m_toolTip = m_toolTip;
    return std::move(clone);
}

void Button::updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset)
{
    auto screenSize = static_cast<sf::Vector2f>(screen.getSize());
    m_scale.setScreenSize(screenSize);
    updateLayout(screenSize);
    m_toolTip.update();

    sf::Vector2f scale(m_scale.getCurrentScale());

    auto& currentPosition = getCurrentPosition();
    sf::IntRect buttonRect(static_cast<int>(currentPosition.x + scale.x * m_style.mouseRect.left),
                           static_cast<int>(currentPosition.y + scale.y * m_style.mouseRect.top),
                           static_cast<int>(scale.x * m_style.mouseRect.width),
                           static_cast<int>(scale.y * m_style.mouseRect.height));
    auto& mousePosition = getCursorPosition(screen);

    if(cursorIsValid() && buttonRect.contains(mousePosition + mouseOffset) && isVisible())
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
            setStyle(m_style.pressedStyle, time, screenSize);

            if(!m_playPressedSound && m_style.pressedStyle.sound)
            {
                m_playPressedSound = true;
                m_style.pressedStyle.sound->play();
            }
        }
        else
        {
            setStyle(m_style.hoverStyle, time, screenSize);
            m_playPressedSound = false;

            if(m_isTriggering && utility::Mouse.leftButtonReleased() && m_callback != nullptr)
                m_callback(*this);
        }
    }
    else
    {
        m_playHoverSound = false;
        m_playPressedSound = false;
        setStyle(m_style.idleStyle, time, screenSize);
        m_showToolTip = false;
    }
    
    m_currentStyle->sprite.setScale(scale);
    m_currentStyle->label.update(screen, time, mouseOffset);
    if(m_currentStyle->animation != nullptr)
    {
        m_currentStyle->animation->setScale(scale);
        m_currentStyle->animation->update(screen, time, mouseOffset);
    }
}

void Button::setStyle(ButtonStateStyle& style, double time, const sf::Vector2f& screenSize)
{
    if(m_currentStyle->animation != style.animation)
    {
        if(m_currentStyle->resetOnExit)
        {
            if(m_style.idleStyle.animation)
                m_style.idleStyle.animation->restartAt(time);
            if(m_style.hoverStyle.animation)
                m_style.hoverStyle.animation->restartAt(time);
            if(m_style.pressedStyle.animation)
                m_style.pressedStyle.animation->restartAt(time);
        }
    }
    m_currentStyle = &style;
    updateLayout(screenSize);
}

void Button::onDrawElement(const DrawParameter& params)
{
    if(m_currentStyle->animation != nullptr)
        m_currentStyle->animation->draw(params);
    else
    {
        m_currentStyle->sprite.draw(params);
        m_currentStyle->label.draw(params);
    }
}

void Button::registerOnPressed(std::function<void (const Button& sender)> callback)
{
    m_callback = callback;
}

void Button::setToolTip(const ToolTip& toolTip)
{
    m_toolTip = toolTip;
}

void Button::setToolTipText(const std::string& text, const std::string& replacement)
{
    m_toolTip.setText(text, replacement);
}

void Button::onDrawAdditionalForeground(const DrawParameter& params)
{
    if(m_showToolTip && isVisible())
        m_toolTip.draw(params);

    m_showToolTip = false;
}

void Button::changeIdleSprite(const Sprite& sprite)
{
    m_style.idleStyle.sprite.setTexture(*sprite.getTexture());
    m_style.idleStyle.sprite.setTextureRect(sprite.getTextureRect());
    m_style.idleStyle.sprite.setOrigin(sprite.getOrigin());
    m_style.idleStyle.sprite.setBlendMode(sprite.getBlendMode());
}

void Button::changeHoverSprite(const Sprite& sprite)
{
    m_style.hoverStyle.sprite.setTexture(*sprite.getTexture());
    m_style.hoverStyle.sprite.setTextureRect(sprite.getTextureRect());
    m_style.hoverStyle.sprite.setOrigin(sprite.getOrigin());
    m_style.hoverStyle.sprite.setBlendMode(sprite.getBlendMode());
}

void Button::changePressedSprite(const Sprite& sprite)
{
    m_style.pressedStyle.sprite.setTexture(*sprite.getTexture());
    m_style.pressedStyle.sprite.setTextureRect(sprite.getTextureRect());
    m_style.pressedStyle.sprite.setOrigin(sprite.getOrigin());
    m_style.pressedStyle.sprite.setBlendMode(sprite.getBlendMode());
}

void Button::layoutUpdated(const sf::Vector2f& screenSize)
{
    MenuElement::layoutUpdated(screenSize);
    
    if(m_currentStyle->animation != nullptr)
        m_currentStyle->animation->updateLayout(screenSize);
    else
    {
        auto half = sf::Vector2f();//sf::Vector2f(getSize().x / 2.f, 0);
        auto currentPosition = getCurrentPosition();
        m_currentStyle->sprite.setPosition(currentPosition + m_currentStyle->spriteOffset - half);
    }
    
    m_currentStyle->label.updateLayout(screenSize);
}
