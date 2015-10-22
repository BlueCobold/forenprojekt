#include "HighScoreMenu.hpp"

#include "../resources/ResourceManager.hpp"

HighScoreMenu::HighScoreMenu(sf::RenderWindow& screen,
                             ResourceManager& resourceManager) :
    Menu(*resourceManager.getMenuTemplate("HighScoreMenu"), screen)
{
}
