#include "LevelPreviewMenu.hpp"

#include "../Utility.hpp"
#include "../gui/MenuSprite.hpp"
#include "../resources/ResourceManager.hpp"

#include "Button.hpp"

LevelPreviewMenu::LevelPreviewMenu(sf::RenderWindow& screen,
                                   ResourceManager& resourceManager) :
    Menu(*resourceManager.getMenuTemplate("LevelPreviewMenu"), screen)
{ }

void LevelPreviewMenu::setLoading(bool loading)
{
    Menu::getLabel(LABEL_BALLS).setVisible(!loading);
    Menu::getLabel(LABEL_BALLS_HINT).setVisible(!loading);
    Menu::getLabel(LABEL_TIME).setVisible(!loading);
    Menu::getLabel(LABEL_TIME_HINT).setVisible(!loading);
    Menu::getLabel(LABEL_LOADING).setVisible(loading);
}

void LevelPreviewMenu::setLevelPreview(const Sprite& sprite)
{
    MenuSprite& item = Menu::getSprite(SPRITE_LEVELINFO);
    item.setSprite(sprite);
}

void LevelPreviewMenu::setLevelInfo(const std::string& levelName,
                                    const double time,
                                    const int balls)
{
    int seconds = static_cast<int>(time) % 60;
    int minutes = static_cast<int>(time) / 60;
    Menu::getLabel(LABEL_LEVELNAME).setText(levelName);

    if(balls <= 0)
        Menu::getLabel(LABEL_BALLS).setText(utility::translateKey("gui_unlimited"));
    else
        Menu::getLabel(LABEL_BALLS).setText(utility::toString(balls));

    if(time <= 0)
        Menu::getLabel(LABEL_TIME).setText(utility::translateKey("gui_unlimited"));
    else
        if(seconds < 10)
            Menu::getLabel(LABEL_TIME).setText(utility::toString(minutes) + ":0" + utility::toString(seconds));
        else
            Menu::getLabel(LABEL_TIME).setText(utility::toString(minutes) + ":" + utility::toString(seconds));
}

void LevelPreviewMenu::setCoinToolTipText(const std::string& text, const std::string& coins)
{
    Menu::getButton(BUTTON_COINS).setToolTipText(text, coins);
}
