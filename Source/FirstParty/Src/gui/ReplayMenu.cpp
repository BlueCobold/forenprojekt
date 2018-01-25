#include "ReplayMenu.hpp"

#include "MenuSprite.hpp"
#include "AnimationContainer.hpp"
#include "../resources/ResourceManager.hpp"

ReplayMenu::ReplayMenu(sf::RenderWindow& screen,
                       ResourceManager& resourceManager) :
    Menu(*resourceManager.getMenuTemplate("replayMenu"), screen)
{
}

void ReplayMenu::onEnter(double time)
{
    Menu::getAnimation(SPRITE_SYMBOL_STAR_BRONCE_FILLED).restartAt(time);
    Menu::getAnimation(SPRITE_SYMBOL_STAR_SILVER_FILLED).restartAt(time);
    Menu::getAnimation(SPRITE_SYMBOL_STAR_GOLD_FILLED).restartAt(time);
}

void ReplayMenu::setPoints(int points)
{
    Menu::getLabel(LABEL_POINTS).setText(utility::toString(points));
}

void ReplayMenu::setGrade(int grade)
{
    Menu::getAnimation(SPRITE_SYMBOL_STAR_BRONCE_FILLED).setVisible(grade <= 2);
    Menu::getAnimation(SPRITE_SYMBOL_STAR_SILVER_FILLED).setVisible(grade <= 1);
    Menu::getAnimation(SPRITE_SYMBOL_STAR_GOLD_FILLED).setVisible(grade <= 0);
    Menu::getSprite(SPRITE_RIBBON_RED).setVisible(grade > 2);
    Menu::getSprite(SPRITE_RIBBON_BRONCE).setVisible(grade == 2);
    Menu::getSprite(SPRITE_RIBBON_SILVER).setVisible(grade == 1);
    Menu::getSprite(SPRITE_RIBBON_GOLD).setVisible(grade == 0);
}

void ReplayMenu::setPlayTime(float playTime)
{
    Menu::getLabel(LABEL_TIME).setText(utility::floatToPlayTimeString(playTime));
}

void ReplayMenu::setLostBalls(int lostBalls)
{
    Menu::getLabel(LABEL_LOSTBALLS).setText(utility::toString(lostBalls));
}

void ReplayMenu::setMedalToolTipText(const std::string& text)
{
    Menu::getSprite(SPRITE_SYMBOL_STAR_BRONCE_DARK).setToolTipText(text);
    Menu::getSprite(SPRITE_SYMBOL_STAR_SILVER_DARK).setToolTipText(text);
    Menu::getSprite(SPRITE_SYMBOL_STAR_GOLD_DARK).setToolTipText(text);
}
