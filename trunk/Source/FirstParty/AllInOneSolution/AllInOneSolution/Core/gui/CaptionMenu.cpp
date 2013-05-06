#include "CaptionMenu.hpp"
#include "../resources/ResourceManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

CaptionMenu::CaptionMenu(const sf::Vector2f& position,
                   const std::string& caption,
                   sf::RenderWindow& screen,
                   ResourceManager& resourceManager,
                   const std::string& backgroundName) :
    Menu(position, screen, resourceManager, backgroundName)
{
    m_caption.setBitmapFont(resourceManager.getBitmapFont("gui_large"));
    m_caption.setPosition(position.x + getSize().x / 2.f, position.y + 40);
    m_caption.setAlignment(LineLabel::Centered);
    m_caption.setText(caption);
}

void CaptionMenu::draw(const DrawParameter& params)
{
    Menu::draw(params);
    m_caption.draw(params);
}

void CaptionMenu::setPosition(const sf::Vector2f& position)
{
    Menu::setPosition(position);
    
    m_caption.setPosition(position.x + getSize().x / 2.f, position.y + 40);
}
