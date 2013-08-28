#include "CoinShopMenu.hpp"
#include "Button.hpp"

#include "../resources/ResourceManager.hpp"

const float CoinShopMenu::SellModifier = 0.8f;

CoinShopMenu::CoinShopMenu(const sf::Vector2f& position,
                           sf::RenderWindow& screen,
                           ResourceManager& resourceManager) :
    CaptionMenu(*resourceManager.getMenuTemplate("CoinShopMenu"), position, screen)
{
    Menu::getButton(BUTTON_GRAVITY_PLUS).setToolTipText(utility::replace(utility::translateKey("tooltip_goody_cost"),
                                                        utility::toString(Price_Gravity)));

    Menu::getButton(BUTTON_GRAVITY_MINUS).setToolTipText(utility::replace(utility::translateKey("tooltip_goody_refund"),
                                                         utility::toString(static_cast<int>(Price_Gravity * SellModifier))));

    Menu::getButton(BUTTON_INVULNERABLE_PLUS).setToolTipText(utility::replace(utility::translateKey("tooltip_goody_cost"),
                                                             utility::toString(Price_Invulnerable)));

    Menu::getButton(BUTTON_INVULNERABLE_MINUS).setToolTipText(utility::replace(utility::translateKey("tooltip_goody_refund"),
                                                              utility::toString(static_cast<int>(Price_Invulnerable * SellModifier))));

    Menu::getButton(BUTTON_EXTRA_BALL_PLUS).setToolTipText(utility::replace(utility::translateKey("tooltip_goody_cost"),
                                                           utility::toString(Price_Extra_Ball)));

    Menu::getButton(BUTTON_EXTRA_BALL_MINUS).setToolTipText(utility::replace(utility::translateKey("tooltip_goody_refund"),
                                                            utility::toString(static_cast<int>(Price_Extra_Ball * SellModifier))));

    Menu::getButton(BUTTON_EXTRA_TIME_PLUS).setToolTipText(utility::replace(utility::translateKey("tooltip_goody_cost"),
                                                           utility::toString(Price_Extra_Time)));

    Menu::getButton(BUTTON_EXTRA_TIME_MINUS).setToolTipText(utility::replace(utility::translateKey("tooltip_goody_refund"),
                                                            utility::toString(static_cast<int>(Price_Extra_Time * SellModifier))));
}

void CoinShopMenu::setGoodyCharges(const Goody::Type& goody, const int charges)
{
    if(goody == Goody::GravityGoody)
        if(charges == -1)
            Menu::getLabel(LABEL_GRAVITY).setText(utility::toString("~"));
        else
            Menu::getLabel(LABEL_GRAVITY).setText(utility::toString<int>(charges));
    else if(goody == Goody::InvulnerableGoody)
        Menu::getLabel(LABEL_INVULNERABLE).setText(utility::toString<int>(charges));
    else if(goody == Goody::ExtraBallGoody)
        Menu::getLabel(LABEL_EXTRA_BALL).setText(utility::toString<int>(charges));
    else if(goody == Goody::ExtraTimeGoody)
        Menu::getLabel(LABEL_EXTRA_TIME).setText(utility::toString<int>(charges));
    else
        throw std::runtime_error(utility::translateKey("InvalidGoody"));

    setGoodyIcon(goody, charges);
}

void CoinShopMenu::setGoodyIcon(const Goody::Type& goody, const int charges)
{
    sf::IntRect textureRect;
    if(charges != 0)
        textureRect = sf::IntRect(goody * 48 + goody * 8, 8, 48 ,48);
    else
        textureRect = sf::IntRect(goody * 48 + goody * 8, 200, 48 ,48);

    if(goody == Goody::GravityGoody)
        Menu::getSprite(SPRITE_GRAVITY).setTextureRect(textureRect);
    else if(goody == Goody::InvulnerableGoody)
        Menu::getSprite(SPRITE_INVULNERABLE).setTextureRect(textureRect);
    else if(goody == Goody::ExtraBallGoody)
        Menu::getSprite(SPRITE_EXTRA_BALL).setTextureRect(textureRect);
    else if(goody == Goody::ExtraTimeGoody)
        Menu::getSprite(SPRITE_EXTRA_TIME).setTextureRect(textureRect);
    else
        throw std::runtime_error(utility::translateKey("InvalidGoody"));
}

int CoinShopMenu::getBuyCost(const Goody::Type& goody) const
{
    if(goody == Goody::GravityGoody)
        return Price_Gravity;
    else if(goody == Goody::InvulnerableGoody)
        return Price_Invulnerable;
    else if(goody == Goody::ExtraBallGoody)
        return Price_Extra_Ball;
    else if(goody == Goody::ExtraTimeGoody)
        return Price_Extra_Time;
    else
        throw std::runtime_error(utility::translateKey("InvalidGoody"));
}

int CoinShopMenu::getSellRefund(const Goody::Type& goody) const
{
    if(goody == Goody::GravityGoody)
        return static_cast<int>(Price_Gravity * SellModifier);
    else if(goody == Goody::InvulnerableGoody)
        return static_cast<int>(Price_Invulnerable * SellModifier);
    else if(goody == Goody::ExtraBallGoody)
        return static_cast<int>(Price_Extra_Ball * SellModifier);
    else if(goody == Goody::ExtraTimeGoody)
        return static_cast<int>(Price_Extra_Time * SellModifier);
    else
        throw std::runtime_error(utility::translateKey("InvalidGoody"));
}
