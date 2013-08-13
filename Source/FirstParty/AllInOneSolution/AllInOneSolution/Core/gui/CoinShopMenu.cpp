#include "CoinShopMenu.hpp"

#include "../resources/ResourceManager.hpp"

CoinShopMenu::CoinShopMenu(const sf::Vector2f& position,
                           sf::RenderWindow& screen,
                           ResourceManager& resourceManager) :
    CaptionMenu(*resourceManager.getMenuTemplate("CoinShopMenu"), position, screen)
{
}

void CoinShopMenu::SetGoodyCharges(const Goody::Type& goody, const int charges)
{
    if(goody == Goody::GravityGoody)
        Menu::getLabel(LABEL_GRAVITY).setText(utility::toString<int>(charges));
    else if(goody == Goody::InvulnerableGoody)
        Menu::getLabel(LABEL_INVULNERABLE).setText(utility::toString<int>(charges));
    else if(goody == Goody::ExtraBallGoody)
        Menu::getLabel(LABEL_EXTRA_BALL).setText(utility::toString<int>(charges));
    else if(goody == Goody::ExtraTimeGoody)
        Menu::getLabel(LABEL_EXTRA_TIME).setText(utility::toString<int>(charges));
}