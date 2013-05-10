#include "ReplayMenu.hpp"

ReplayMenu::ReplayMenu(const sf::Vector2f& position,
                       sf::RenderWindow& screen,
                       ResourceManager& resourceManager) :
    CaptionMenu(*resourceManager.getMenuTemplate("replayMenu"), position, screen)
{
}
