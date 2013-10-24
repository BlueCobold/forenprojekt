#include "HighScoreMenu.hpp"

#include "../resources/ResourceManager.hpp"

HighScoreMenu::HighScoreMenu(const sf::Vector2f& position,
                             sf::RenderWindow& screen,
                             ResourceManager& resourceManager) :
    CaptionMenu(*resourceManager.getMenuTemplate("HighScoreMenu"), position, screen)
{
}
