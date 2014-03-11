#include "CaptionMenu.hpp"
#include "../resources/ResourceManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

CaptionMenu::CaptionMenu(MenuTemplate& menuTemplate,
                         const sf::Vector2f& position,
                         sf::RenderWindow& screen) :
    Menu(menuTemplate, position, screen),
    m_caption(utility::translateKey(menuTemplate.captionResourceKey),
              sf::Vector2f(position.x + getSize().x / 2.f, position.y),
              menuTemplate.captionOffset,
              0,
              menuTemplate.captionFont,
              LineLabel::Centered)
{
}

void CaptionMenu::drawAdditionalBackground(const DrawParameter& params)
{
    m_caption.draw(params);
}

void CaptionMenu::setPosition(const sf::Vector2f& position)
{
    Menu::setPosition(position);

    m_caption.setPosition(position.x + getSize().x / 2.f, position.y);
}

void CaptionMenu::setCaption(const std::string& caption)
{
    m_caption.setText(caption);
}
