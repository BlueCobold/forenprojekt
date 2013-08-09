#include "CoinShopMenu.hpp"

#include "../resources/ResourceManager.hpp"

CoinShopMenu::CoinShopMenu(const sf::Vector2f& position,
                           sf::RenderWindow& screen,
                           ResourceManager& resourceManager) :
    CaptionMenu(*resourceManager.getMenuTemplate("CoinShopMenu"), position, screen)
{
}