#include "HighScoreMenu.hpp"

#include "../resources/ResourceManager.hpp"

HighScoreMenu::HighScoreMenu(sf::RenderWindow& screen,
                             ResourceManager& resourceManager) :
    CaptionMenu(*resourceManager.getMenuTemplate("HighScoreMenu"), screen)
{
}
