#include "ReplayMenu.hpp"

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
    Menu::getSprite(SPRITE_SYMBOL_NONE).setVisible(grade == 3);
    Menu::getSprite(SPRITE_SYMBOL_BRONCE).setVisible(grade == 2);
    Menu::getSprite(SPRITE_SYMBOL_SILVER).setVisible(grade == 1);
    Menu::getSprite(SPRITE_SYMBOL_GOLD).setVisible(grade == 0);
}
void ReplayMenu::setLostBalls(int lostBalls)
{
    Menu::getLabel(LABEL_LOSTBALLS).setText(utility::toString(lostBalls));
}