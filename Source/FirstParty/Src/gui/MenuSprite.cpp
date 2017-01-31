#include "MenuSprite.hpp"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

MenuSprite::MenuSprite(const Sprite& sprite,
                       const ScreenLocation& position,
                       const ScreenSize& size,
                       const int id) :
    MenuElement(id, MenuElementType::Image, position),
    SizedElement(size),
    m_sprite(sprite),
    m_scale(sf::Vector2f(1, 1)),
    m_keepAspectRatio(false),
    m_showToolTip(false)
{
}

std::unique_ptr<MenuElement> MenuSprite::doClone() const
{
    auto clone = std::unique_ptr<MenuSprite>(new MenuSprite(m_sprite, getPosition(), getSize(), getId()));
    clone->setVisibleWhenId(getVisibleWhenId());
    clone->m_toolTip = m_toolTip;
    clone->setScale(m_scale, m_keepAspectRatio);
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
    auto screenSize = static_cast<sf::Vector2f>(screen.getSize());
    setScreenSize(screenSize);
    updateLayout(screenSize);

    sf::IntRect rect = sf::IntRect(static_cast<int>(m_sprite.getPosition().x),
                                   static_cast<int>(m_sprite.getPosition().y),
                                   m_sprite.getTextureRect().width,
                                   m_sprite.getTextureRect().height);

    auto mousePosition = getCursorPosition(screen);
    if(cursorIsValid() && rect.contains(mousePosition + mouseOffset) && isVisible())
    {
        m_showToolTip = true;
        m_toolTip.setPosition(static_cast<const sf::Vector2f>(mousePosition), screen);
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

void MenuSprite::setScale(const sf::Vector2f& scale, bool keepAspectRatio)
{
    m_scale = scale;
    m_keepAspectRatio = keepAspectRatio;
}

void MenuSprite::onDrawAdditionalForeground(const DrawParameter& params)
{
    if(m_showToolTip && isVisible())
        m_toolTip.draw(params);

    m_showToolTip = false;
}

void MenuSprite::layoutUpdated(const sf::Vector2f& screenSize)
{
    m_sprite.setPosition(sf::Vector2f(floorf(getCurrentPosition().x), floorf(getCurrentPosition().y)));
    auto& texRect = m_sprite.getTextureRect();
    auto& size = getCurrentSize();
    auto scaleX = size.x / texRect.width * m_scale.x;
    if(m_keepAspectRatio)
        m_sprite.setScale(scaleX, scaleX);
    else
        m_sprite.setScale(scaleX, size.y / texRect.height * m_scale.y);
}
