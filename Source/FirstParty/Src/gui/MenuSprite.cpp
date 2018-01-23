#include "MenuSprite.hpp"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

MenuSprite::MenuSprite(const Sprite& sprite,
                       const ScreenLocation& position,
                       const ScreenScale& scale,
                       const int id) :
    MenuElement(id, MenuElementType::Image, position),
    m_sprite(sprite),
    m_scale(scale),
    m_showToolTip(false)
{
}

std::unique_ptr<MenuElement> MenuSprite::onClone() const
{
    auto clone = std::unique_ptr<MenuSprite>(new MenuSprite(m_sprite, getPosition(), m_scale, getId()));
    clone->m_toolTip = m_toolTip;
    return std::move(clone);
}

void MenuSprite::onDrawElement(const DrawParameter& params)
{
    m_sprite.draw(params);
}

void MenuSprite::setToolTip(const ToolTip& toolTip)
{
    m_toolTip = toolTip;
}

void MenuSprite::updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset)
{
    auto screenSize = sf::Vector2f(screen.getSize());
    updateLayout(screenSize);

    auto scale = m_scale.getCurrentScale();
    sf::FloatRect rect(m_sprite.getPosition().x - m_sprite.getOrigin().x * scale.x,
                       m_sprite.getPosition().y - m_sprite.getOrigin().y * scale.y,
                       m_sprite.getTextureRect().width * scale.x,
                       m_sprite.getTextureRect().height * scale.y);

    auto mousePosition = getCursorPosition(screen);
    if(cursorIsValid() && rect.contains(sf::Vector2f(mousePosition + mouseOffset)) && isVisible())
    {
        m_showToolTip = true;
        m_toolTip.setPosition(sf::Vector2f(mousePosition), screen);
        m_toolTip.update();
    }
    else
        m_showToolTip = false;  
}

void MenuSprite::setToolTipText(const std::string& text, const std::string& replacement)
{
    m_toolTip.setText(text, replacement);
}

void MenuSprite::setTextureRect(const sf::IntRect& textureRect)
{
    m_sprite.setTextureRect(textureRect);
}

void MenuSprite::setTexture(const sf::Texture& texture)
{
    m_sprite.setTexture(texture);
}

void MenuSprite::setSprite(const Sprite& sprite)
{
    m_sprite.setTextureRect(sprite.getTextureRect());
    m_sprite.setOrigin(sprite.getOrigin());
    m_sprite.setTexture(*sprite.getTexture());
    m_sprite.setBlendMode(sprite.getBlendMode());
}

void MenuSprite::onDrawAdditionalForeground(const DrawParameter& params)
{
    if(m_showToolTip && isVisible())
        m_toolTip.draw(params);

    m_showToolTip = false;
}

void MenuSprite::layoutUpdated(const sf::Vector2f& screenSize)
{
    m_scale.setScreenSize(screenSize);
    auto scale = m_scale.getCurrentScale();
    m_sprite.setScale(scale);

    m_sprite.setPosition(
        sf::Vector2f(
            floorf(getCurrentPosition().x),
            floorf(getCurrentPosition().y)));
}
