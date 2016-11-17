#include "MainMenu.hpp"

#include "../resources/ResourceManager.hpp"
#include "Button.hpp"
#include "SubWindow.hpp"

MainMenu::MainMenu(sf::RenderWindow& screen,
                   ResourceManager& resourceManager) :
    Menu(*resourceManager.getMenuTemplate("MainMenu"), screen)
{
}

void MainMenu::showGermanLanguage() const
{
    Menu::getButton(BUTTON_GERMAN_SHOW).setVisible(true);
    Menu::getButton(BUTTON_ENGLISH_SHOW).setVisible(false);
}

void MainMenu::showEnglishLanguage() const
{
    Menu::getButton(BUTTON_ENGLISH_SHOW).setVisible(true);
    Menu::getButton(BUTTON_GERMAN_SHOW).setVisible(false);
}

void MainMenu::showLanguageSelect(bool show) const
{
    Menu::getSubWindow(SUBWINDOW_SELECT_LANGUAGE).setVisible(show);
}
