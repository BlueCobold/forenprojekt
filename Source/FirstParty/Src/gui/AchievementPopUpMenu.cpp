#include "AchievementPopUpMenu.hpp"
#include "../resources/ResourceManager.hpp"

AchievementPopUpMenu::AchievementPopUpMenu(sf::RenderWindow& screen,
                                 ResourceManager& resourceManager) :
    Menu(*resourceManager.getMenuTemplate("newAchivementMenu"), screen)
{
}