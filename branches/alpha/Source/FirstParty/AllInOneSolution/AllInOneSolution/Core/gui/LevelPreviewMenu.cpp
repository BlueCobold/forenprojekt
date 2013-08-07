#include "LevelPreviewMenu.hpp"

#include "../resources/ResourceManager.hpp"
#include "../Utility.hpp"

LevelPreviewMenu::LevelPreviewMenu(const sf::Vector2f& position,
                                   sf::RenderWindow& screen,
                                   ResourceManager& resourceManager) :
    Menu(*resourceManager.getMenuTemplate("LevelPreviewMenu"), position, screen)
{
}

void LevelPreviewMenu::setLevelInfo(const std::string& levelName,
                                    const float time,
                                    const int balls)
{
    int seconds = static_cast<int>(time) % 60;
    int minutes = static_cast<int>(time) / 60;
    Menu::getLabel(LABEL_LEVELNAME).setText(levelName);

    if(balls == -1)
        Menu::getLabel(LABEL_BALLS).setText(utility::translateKey("gui_unlimited"));
    else
        Menu::getLabel(LABEL_BALLS).setText(utility::toString(balls));

    if(time < 0)
        Menu::getLabel(LABEL_TIME).setText(utility::translateKey("gui_unlimited"));
    else
        if(seconds < 10)
            Menu::getLabel(LABEL_TIME).setText(utility::toString(minutes) + ":0" + utility::toString(seconds));
        else
            Menu::getLabel(LABEL_TIME).setText(utility::toString(minutes) + ":" + utility::toString(seconds));
}

void LevelPreviewMenu::setCoinToolTipText(const std::string& text)
{
    Menu::getSprite(SPRITE_COINS).setToolTipText(text);
}
