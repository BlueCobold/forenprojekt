#include "CaptionMenu.hpp"
#include "../resources/ResourceManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

CaptionMenu::CaptionMenu(const MenuTemplate& menuTemplate,
                         const sf::Vector2f& position,
                         sf::RenderWindow& screen) :
    Menu(menuTemplate, position, screen)
{
    m_caption.setBitmapFont(menuTemplate.captionFont);
    m_caption.setPosition(position.x + getSize().x / 2.f, position.y);
    m_caption.setOffset(menuTemplate.captionOffset);
    m_caption.setAlignment(LineLabel::Centered);
    m_caption.setText(utility::translateKey(menuTemplate.captionResourceKey));
}

void CaptionMenu::draw(const DrawParameter& params)
{
    Menu::draw(params);
    m_caption.draw(params);
}

void CaptionMenu::setPosition(const sf::Vector2f& position)
{
    Menu::setPosition(position);
    
    m_caption.setPosition(position.x + getSize().x / 2.f, position.y);
}
