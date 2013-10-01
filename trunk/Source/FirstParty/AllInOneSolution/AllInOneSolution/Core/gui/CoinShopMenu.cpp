#include "CoinShopMenu.hpp"
#include "Button.hpp"

#include "../resources/ResourceManager.hpp"

const float CoinShopMenu::SellModifier = 0.8f;

CoinShopMenu::CoinShopMenu(const sf::Vector2f& position,
                           sf::RenderWindow& screen,
                           ResourceManager& resourceManager) :
    CaptionMenu(*resourceManager.getMenuTemplate("CoinShopMenu"), position, screen)
{
    setToolTip(BUTTON_GRAVITY_PLUS, "tooltip_goody_cost", Price_Gravity);
    setToolTip(BUTTON_GRAVITY_MINUS, "tooltip_goody_refund", static_cast<int>(Price_Gravity * SellModifier));
    setToolTip(BUTTON_INVULNERABLE_PLUS, "tooltip_goody_cost", Price_Invulnerable);
    setToolTip(BUTTON_INVULNERABLE_MINUS, "tooltip_goody_refund", static_cast<int>(Price_Invulnerable * SellModifier));
    setToolTip(BUTTON_EXTRA_BALL_PLUS, "tooltip_goody_cost", Price_Invulnerable);
    setToolTip(BUTTON_EXTRA_BALL_MINUS, "tooltip_goody_refund", static_cast<int>(Price_Invulnerable * SellModifier));
    setToolTip(BUTTON_EXTRA_TIME_PLUS, "tooltip_goody_cost", Price_Extra_Time);
    setToolTip(BUTTON_EXTRA_TIME_MINUS, "tooltip_goody_refund", static_cast<int>(Price_Extra_Time * SellModifier));
}

void CoinShopMenu::setToolTip(int id, const std::string& textKey, int costs)
{
    Menu::getButton(id).setToolTipText(utility::replace(utility::translateKey(textKey),
                                                        utility::toString(costs)));
}

void CoinShopMenu::setGoodyCharges(const Goody::Type& goody, const int charges)
{
    setGoodyIcon(goody, charges);
    switch(goody)
    {
    case Goody::GravityGoody:
        if(charges == -1)
            return Menu::getLabel(LABEL_GRAVITY).setText(utility::toString("~"));
        else
            return Menu::getLabel(LABEL_GRAVITY).setText(utility::toString<int>(charges));
    case Goody::InvulnerableGoody:
        return Menu::getLabel(LABEL_INVULNERABLE).setText(utility::toString<int>(charges));
    case Goody::ExtraBallGoody:
        return Menu::getLabel(LABEL_EXTRA_BALL).setText(utility::toString<int>(charges));
    case Goody::ExtraTimeGoody:
        return Menu::getLabel(LABEL_EXTRA_TIME).setText(utility::toString<int>(charges));
    default:
        throw std::runtime_error(utility::translateKey("InvalidGoody"));
    }
}

void CoinShopMenu::setGoodyIcon(const Goody::Type& goody, const int charges)
{
    sf::IntRect textureRect;
    if(charges != 0)
        textureRect = sf::IntRect(goody * 48 + goody * 8, 8, 48 ,48);
    else
        textureRect = sf::IntRect(goody * 48 + goody * 8, 200, 48 ,48);

    switch(goody)
    {
    case Goody::GravityGoody:
        return Menu::getSprite(SPRITE_GRAVITY).setTextureRect(textureRect);
    case Goody::InvulnerableGoody:
        return Menu::getSprite(SPRITE_INVULNERABLE).setTextureRect(textureRect);
    case Goody::ExtraBallGoody:
        return Menu::getSprite(SPRITE_EXTRA_BALL).setTextureRect(textureRect);
    case Goody::ExtraTimeGoody:
        return Menu::getSprite(SPRITE_EXTRA_TIME).setTextureRect(textureRect);
    default:
        throw std::runtime_error(utility::translateKey("InvalidGoody"));
    }
}

int CoinShopMenu::getBuyCost(const Goody::Type& goody) const
{
    switch(goody)
    {
    case Goody::GravityGoody:
        return Price_Gravity;
    case Goody::InvulnerableGoody:
        return Price_Invulnerable;
    case Goody::ExtraBallGoody:
        return Price_Extra_Ball;
    case Goody::ExtraTimeGoody:
        return Price_Extra_Time;
    default:
        throw std::runtime_error(utility::translateKey("InvalidGoody"));
    }
}

int CoinShopMenu::getSellRefund(const Goody::Type& goody) const
{
    switch(goody)
    {
    case Goody::GravityGoody:
        return static_cast<int>(Price_Gravity * SellModifier);
    case Goody::InvulnerableGoody:
        return static_cast<int>(Price_Invulnerable * SellModifier);
    case Goody::ExtraBallGoody:
        return static_cast<int>(Price_Extra_Ball * SellModifier);
    case Goody::ExtraTimeGoody:
        return static_cast<int>(Price_Extra_Time * SellModifier);
    default:
        throw std::runtime_error(utility::translateKey("InvalidGoody"));
    }
}
