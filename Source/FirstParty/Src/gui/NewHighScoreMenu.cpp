#include "NewHighScoreMenu.hpp"

#include "../resources/ResourceManager.hpp"

NewHighScoreMenu::NewHighScoreMenu(sf::RenderWindow& screen,
                                   ResourceManager& resourceManager) :
    Menu(*resourceManager.getMenuTemplate("NewHighScoreMenu"), screen),
    m_virtualKeyboard(screen, resourceManager)
{
}

void NewHighScoreMenu::onBeforeUpdate(sf::RenderWindow& screen, const double time)
{
    m_virtualKeyboard.update(screen, time);
}

void NewHighScoreMenu::doDraw(const DrawParameter& params)
{
    Menu::doDraw(params);
    m_virtualKeyboard.draw(params);
}

void NewHighScoreMenu::showKeyboard()
{
    m_virtualKeyboard.showKeyboard();
}

bool NewHighScoreMenu::isKeyboardShown()
{
    return m_virtualKeyboard.isShown();
}