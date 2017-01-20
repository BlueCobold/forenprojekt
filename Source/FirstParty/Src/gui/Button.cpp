#include "Button.hpp"
#include "../Input.hpp"

#include <SFML/Window/Event.hpp>

Button::Button(int id, ButtonStyle style,
               const ScreenLocation& position,
               bool triggers) :
    MenuElement(id, MenuElementType::Button, position),
    m_style(std::move(style)),
    m_showToolTip(false),
    m_isTriggering(triggers),
    m_playHoverSound(false),
    m_playPressedSound(false)
{
    m_currentStyle = &m_style.idleStyle;

    m_size.x = m_style.idleStyle.sprite.getTextureRect().width;
    m_size.y = m_style.idleStyle.sprite.getTextureRect().height;

    if(m_style.idleStyle.animation != nullptr)
        m_style.idleStyle.animation->setPosition(ScreenLocation(position).addOffset(m_style.idleStyle.spriteOffset));

    if(m_style.hoverStyle.animation != nullptr)
        m_style.hoverStyle.animation->setPosition(ScreenLocation(position).addOffset(m_style.hoverStyle.spriteOffset));

    if(m_style.pressedStyle.animation != nullptr)
        m_style.pressedStyle.animation->setPosition(ScreenLocation(position).addOffset(m_style.pressedStyle.spriteOffset));
}

std::unique_ptr<MenuElement> Button::doClone() const
{
    auto clone = std::unique_ptr<Button>(new Button(getId(), m_style, getPosition(), m_isTriggering));
    clone->setVisibleWhenId(getVisibleWhenId());
    clone->m_toolTip = m_toolTip;
    return std::move(clone);
}

void Button::updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset)
{
    auto screenSize = static_cast<sf::Vector2f>(screen.getSize());
    updateLayout(screenSize);
    m_toolTip.update();

    auto currentPosition = getCurrentPosition();
    sf::IntRect buttonRect(static_cast<int>(currentPosition.x + m_style.mouseRect.left - getSize().x / 2),
                            static_cast<int>(currentPosition.y + m_style.mouseRect.top),
                            m_style.mouseRect.width,
                            m_style.mouseRect.height);
    auto mousePosition = getCursorPosition(screen);

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
            m_style.idleStyle.animation->restartAt(time);
            m_style.hoverStyle.animation->restartAt(time);
            m_style.pressedStyle.animation->restartAt(time);
        }
    }
    m_currentStyle = &style;
    updateLayout(screenSize);
}

void Button::doDraw(const DrawParameter& params)
{
    if(!isVisible())
        return;

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

const sf::Vector2i& Button::getSize() const
{
    return m_size;
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

void Button::layoutUpdated(const sf::Vector2f& screenSize)
{
    MenuElement::layoutUpdated(screenSize);
    
    if(m_currentStyle->animation != nullptr)
        m_currentStyle->animation->updateLayout(screenSize);
    else
    {
        auto half = sf::Vector2f(getSize().x / 2.f, 0);
        auto currentPosition = getCurrentPosition();
        m_currentStyle->sprite.setPosition(currentPosition + m_currentStyle->spriteOffset - half);
    }
    
    m_currentStyle->label.updateLayout(screenSize);
}
