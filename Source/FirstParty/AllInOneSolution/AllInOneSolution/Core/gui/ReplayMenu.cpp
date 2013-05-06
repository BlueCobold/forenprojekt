#include "ReplayMenu.hpp"

ReplayMenu::ReplayMenu(const sf::Vector2f& position,
                       sf::RenderWindow& screen,
                       ResourceManager& resourceManager) :
    CaptionMenu(position, utility::translateKey("gui_level_success"), screen, resourceManager, "Window")
{
    createButton(position, sf::Vector2f(getSize().x / 2.f, 100.f), utility::translateKey("gui_play_again"), BUTTON_PLAY_AGAIN);
}
