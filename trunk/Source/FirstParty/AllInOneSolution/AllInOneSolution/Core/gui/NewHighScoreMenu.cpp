#include "NewHighScoreMenu.hpp"

#include "../resources/ResourceManager.hpp"

NewHighScoreMenu::NewHighScoreMenu(const sf::Vector2f& position,
                                   sf::RenderWindow& screen,
                                   ResourceManager& resourceManager) :
    Menu(*resourceManager.getMenuTemplate("NewHighScoreMenu"), position, screen)
{
}
