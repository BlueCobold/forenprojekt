#include "CaptionMenu.hpp"
#include "../resources/ResourceManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

CaptionMenu::CaptionMenu(MenuTemplate& menuTemplate,
                         sf::RenderWindow& screen) :
    Menu(menuTemplate, screen),
    m_caption(utility::translateKey(menuTemplate.captionResourceKey),
              Menu::getPosition(),
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

void CaptionMenu::setPosition(const sf::Vector2f& offset,
                              float horizontalPercentage,
                              float verticalPercentage)
{
    Menu::setPosition(offset, horizontalPercentage, verticalPercentage);

    m_caption.setPosition(sf::Vector2f(getSize().x * horizontalPercentage, getSize().y * verticalPercentage) + offset);
}

void CaptionMenu::setCaption(const std::string& caption)
{
    m_caption.setText(caption);
}

void CaptionMenu::updateLayout()
{
    Menu::updateLayout();
    m_caption.setPosition(Menu::getPosition());
}