#include "CoinShopMenu.hpp"

#include "../resources/ResourceManager.hpp"

CoinShopMenu::CoinShopMenu(const sf::Vector2f& position,
                           sf::RenderWindow& screen,
                           ResourceManager& resourceManager) :
    CaptionMenu(*resourceManager.getMenuTemplate("CoinShopMenu"), position, screen)
{
}

void CoinShopMenu::setGoodyCharges(const Goody::Type& goody, const int charges)
{
    if(goody == Goody::GravityGoody)
        Menu::getLabel(LABEL_GRAVITY).setText(utility::toString<int>(charges));
    else if(goody == Goody::InvulnerableGoody)
        Menu::getLabel(LABEL_INVULNERABLE).setText(utility::toString<int>(charges));
    else if(goody == Goody::ExtraBallGoody)
        Menu::getLabel(LABEL_EXTRA_BALL).setText(utility::toString<int>(charges));
    else if(goody == Goody::ExtraTimeGoody)
        Menu::getLabel(LABEL_EXTRA_TIME).setText(utility::toString<int>(charges));

    setGoodyIcon(goody, charges);
}

void CoinShopMenu::setGoodyIcon(const Goody::Type& goody, const int charges)
{
    sf::IntRect textureRect;
    if(charges != 0)
        textureRect = sf::IntRect(goody * 56, Deselected * 64, 56 ,64);
    else
        textureRect = sf::IntRect(goody * 56, Disabled * 64, 56 ,64);

    if(goody == Goody::GravityGoody)
        Menu::getSprite(SPRITE_GRAVITY).setTextureRect(textureRect);
    else if(goody == Goody::InvulnerableGoody)
        Menu::getSprite(SPRITE_INVULNERABLE).setTextureRect(textureRect);
    else if(goody == Goody::ExtraBallGoody)
        Menu::getSprite(SPRITE_EXTRA_BALL).setTextureRect(textureRect);
    else if(goody == Goody::ExtraTimeGoody)
        Menu::getSprite(SPRITE_EXTRA_TIME).setTextureRect(textureRect);
}