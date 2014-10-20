#include "ReplayMenu.hpp"

#include "../resources/ResourceManager.hpp"

ReplayMenu::ReplayMenu(const sf::Vector2f& position,
                       sf::RenderWindow& screen,
                       ResourceManager& resourceManager) :
    CaptionMenu(*resourceManager.getMenuTemplate("replayMenu"), position, screen)
{
}

void ReplayMenu::setPoints(int points)
{
    Menu::getLabel(LABEL_POINTS).setText(utility::toString(points));
}

void ReplayMenu::setGrade(int grade)
{
    Menu::getSprite(SPRITE_SYMBOL_STAR_BRONCE_DARK).setVisible(grade > 2);
    Menu::getSprite(SPRITE_SYMBOL_STAR_BRONCE_FILLED).setVisible(grade <= 2);
    Menu::getSprite(SPRITE_SYMBOL_STAR_SILVER_DARK).setVisible(grade > 1);
    Menu::getSprite(SPRITE_SYMBOL_STAR_SILVER_FILLED).setVisible(grade <= 1);
    Menu::getSprite(SPRITE_SYMBOL_STAR_GOLD_DARK).setVisible(grade > 0);
    Menu::getSprite(SPRITE_SYMBOL_STAR_GOLD_FILLED).setVisible(grade <= 0);
}

void ReplayMenu::setLostBalls(int lostBalls)
{
    Menu::getLabel(LABEL_LOSTBALLS).setText(utility::toString(lostBalls));
}

void ReplayMenu::setMedalToolTipText(const std::string& text)
{
    Menu::getSprite(SPRITE_SYMBOL_STAR_BRONCE_DARK).setToolTipText(text);
    Menu::getSprite(SPRITE_SYMBOL_STAR_BRONCE_FILLED).setToolTipText(text);
    Menu::getSprite(SPRITE_SYMBOL_STAR_SILVER_DARK).setToolTipText(text);
    Menu::getSprite(SPRITE_SYMBOL_STAR_SILVER_FILLED).setToolTipText(text);
    Menu::getSprite(SPRITE_SYMBOL_STAR_GOLD_DARK).setToolTipText(text);
    Menu::getSprite(SPRITE_SYMBOL_STAR_GOLD_FILLED).setToolTipText(text);
}
