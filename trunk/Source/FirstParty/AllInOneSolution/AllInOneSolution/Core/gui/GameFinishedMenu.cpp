#include "GameFinishedMenu.hpp"

#include "../resources/ResourceManager.hpp"

GameFinishedMenu::GameFinishedMenu(sf::RenderWindow& screen,
                                   ResourceManager& resourceManager) :
    Menu(*resourceManager.getMenuTemplate("GameFinishedMenu"), screen)
{
}