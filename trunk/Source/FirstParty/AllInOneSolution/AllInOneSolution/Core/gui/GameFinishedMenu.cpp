#include "GameFinishedMenu.hpp"

#include "../resources/ResourceManager.hpp"

GameFinishedMenu::GameFinishedMenu(const sf::Vector2f& position,
                       sf::RenderWindow& screen,
                       ResourceManager& resourceManager) :
    Menu(*resourceManager.getMenuTemplate("GameFinishedMenu"), position, screen)
{
}