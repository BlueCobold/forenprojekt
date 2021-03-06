#include "CaptionMenu.hpp"
#include "../resources/ResourceManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

CaptionMenu::CaptionMenu(const MenuTemplate& menuTemplate,
                         sf::RenderWindow& screen) :
    Menu(menuTemplate, screen)
{
}

void CaptionMenu::drawAdditionalBackground(const DrawParameter& params)
{
    //m_caption.draw(params);
}

/*void CaptionMenu::setPosition(const sf::Vector2f& relativePosition,
                              const sf::Vector2f& offset)
{
    Menu::setPosition(relativePosition, offset);

    m_caption.setPosition(sf::Vector2f(getSize().x * relativePosition.x,
                                       getSize().y * relativePosition.y) + offset);
}*/

void CaptionMenu::setCaption(const std::string& caption)
{
    //m_caption.setText(caption);
}

