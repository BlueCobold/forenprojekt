#include "AchievementMenu.hpp"
#include "Button.hpp"
#include "../resources/ResourceManager.hpp"

AchievementMenu::AchievementMenu(sf::RenderWindow& screen,
                                 ResourceManager& resourceManager) :
    Menu(*resourceManager.getMenuTemplate("AchievementMenu"), screen)
{
}

void AchievementMenu::hideLeftButton(const bool value)
{
    Menu::getButton(BUTTON_LEFT).setVisible(!value);
}

void AchievementMenu::hideRightButton(const bool value)
{
    Menu::getButton(BUTTON_RIGHT).setVisible(!value);
}