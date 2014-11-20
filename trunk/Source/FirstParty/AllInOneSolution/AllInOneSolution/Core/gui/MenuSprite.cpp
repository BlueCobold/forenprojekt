#include "MenuSprite.hpp"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

MenuSprite::MenuSprite(const sf::Sprite& sprite, const sf::Vector2f& position, const sf::Vector2f& offset, const int id) :
    MenuElement(id, MenuElementType::Image, position, offset),
    m_sprite(sprite),
    m_showToolTip(false)
{
    onPositionChanged();
}

void MenuSprite::draw(const DrawParameter& params)
{
    if(!isVisible())
        return;

    params.getTarget().draw(m_sprite);
}

void MenuSprite::setToolTip(const ToolTip& toolTip)
{
    m_toolTip = toolTip;
}

void MenuSprite::onPositionChanged()
{
    m_sprite.setPosition(getPosition() + getOffset());
}

void MenuSprite::update(const sf::RenderWindow& screen, const float time, const sf::Vector2i& mouseOffset)
{
    sf::IntRect rect = sf::IntRect(static_cast<int>(m_sprite.getPosition().x),
                                   static_cast<int>(m_sprite.getPosition().y),
                                   m_sprite.getTextureRect().width,
                                   m_sprite.getTextureRect().height);

    auto mousePosition = getCursorPosition(screen);
    if(rect.contains(mousePosition + mouseOffset) && isVisible())
    {
        m_showToolTip = true;
        m_toolTip.setPosition(static_cast<const sf::Vector2f>(mousePosition), screen);
    }
    else
        m_showToolTip = false;  
}

void MenuSprite::setToolTipText(const std::string& text)
{
    m_toolTip.setText(text);
}

void MenuSprite::setTextureRect(const sf::IntRect& textureRect)
{
    m_sprite.setTextureRect(textureRect);
}

void MenuSprite::setTexture(const sf::Texture& texture)
{
    m_sprite.setTexture(texture);
}

void MenuSprite::drawAdditionalForeground(const DrawParameter& params)
{
    if(m_showToolTip && isVisible())
        m_toolTip.draw(params);

    m_showToolTip = false;
}
