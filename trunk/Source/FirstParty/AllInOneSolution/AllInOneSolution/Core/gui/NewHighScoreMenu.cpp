#include "NewHighScoreMenu.hpp"

#include "../resources/ResourceManager.hpp"

NewHighScoreMenu::NewHighScoreMenu(const sf::Vector2f& position,
                                   sf::RenderWindow& screen,
                                   ResourceManager& resourceManager) :
    Menu(*resourceManager.getMenuTemplate("NewHighScoreMenu"), position, screen),
    m_virtualKeyboard(position, screen, resourceManager)
{
}

void NewHighScoreMenu::update(sf::RenderWindow& screen, const float time)
{
    m_virtualKeyboard.update(screen, time);
    Menu::update(screen, time);
}

void NewHighScoreMenu::draw(const DrawParameter& params)
{
    Menu::draw(params);
    m_virtualKeyboard.draw(params);
}

void NewHighScoreMenu::showKeyboard()
{
    m_virtualKeyboard.showKeyboard();
}