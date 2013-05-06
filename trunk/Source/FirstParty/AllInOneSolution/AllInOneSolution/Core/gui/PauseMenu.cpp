#include "PauseMenu.hpp"

PauseMenu::PauseMenu(const sf::Vector2f& position,
                   sf::RenderWindow& screen,
                   ResourceManager& resourceManager) :
    CaptionMenu(position, utility::translateKey("gui_game_paused"), screen, resourceManager, "Window")
{
    createButton(position, sf::Vector2f(getSize().x / 2.f, 100.f), utility::translateKey("gui_continue"), BUTTON_CONTINUE);
}
