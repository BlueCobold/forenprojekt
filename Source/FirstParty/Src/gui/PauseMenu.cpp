#include "PauseMenu.hpp"

#include "../resources/ResourceManager.hpp"
#include "Button.hpp"

PauseMenu::PauseMenu(sf::RenderWindow& screen,
                     ResourceManager& resourceManager) :
    CaptionMenu(*resourceManager.getMenuTemplate("pauseMenu"), screen)
{
}

void PauseMenu::setCoinToolTipText(const std::string& text)
{
    //Menu::getSprite(SPRITE_COINS).setToolTipText(text);
    Menu::getButton(BUTTON_COINS).setToolTipText(text);
}