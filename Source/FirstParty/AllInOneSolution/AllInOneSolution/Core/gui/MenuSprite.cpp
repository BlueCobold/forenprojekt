#include "MenuSprite.hpp"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

MenuSprite::MenuSprite() :
    m_position(sf::Vector2f(0, 0)),
    m_offset(sf::Vector2f(0, 0)),
    m_id(-1),
    m_visible(true),
    m_showToolTip(false)
{
}

MenuSprite::MenuSprite(const sf::Vector2f& position, const sf::Vector2f& offset, const int id) :
    m_position(position),
    m_offset(offset),
    m_id(id),
    m_visible(true),
    m_showToolTip(false)
{
}
    
void MenuSprite::setOffset(const sf::Vector2f& offset)
{
    m_offset = offset;
    sf::Sprite::setPosition(m_offset + m_position);
}

sf::Vector2f MenuSprite::getOffset()
{
    return m_offset;
}

void MenuSprite::setPosition(const sf::Vector2f& position)
{
    m_position = position;
    sf::Sprite::setPosition(m_offset + m_position);
}

sf::Vector2f MenuSprite::getPosition()
{
    return m_position;
}

void MenuSprite::draw(const DrawParameter& params)
{
    if(m_visible)
    {
        params.getTarget().draw(*this);
        if(m_showToolTip)
            m_toolTip.draw(params);
    }
}

void MenuSprite::setVisible(const bool visible)
{
    m_visible = visible;
}

void MenuSprite::setId(const int id)
{
    m_id = id;
}

int MenuSprite::getId()
{
    return m_id;
}
void MenuSprite::setToolTip(ToolTip& toolTip)
{
    m_toolTip = toolTip;
}

void MenuSprite::update(const sf::RenderWindow& screen)
{
    sf::IntRect rect = sf::IntRect(static_cast<int>(Sprite::getPosition().x),
                                   static_cast<int>(Sprite::getPosition().y),
                                   Sprite::getTextureRect().width,
                                   Sprite::getTextureRect().height);

    sf::Vector2i mouseposition = sf::Mouse::getPosition(screen);
    if(rect.contains(mouseposition))
    {
        m_showToolTip = true;
        m_toolTip.setPosition(static_cast<const sf::Vector2f>(mouseposition));
    }
    else
        m_showToolTip = false;  
}
void MenuSprite::setToolTipText(const std::string& text)
{
    m_toolTip.setText(text);
}