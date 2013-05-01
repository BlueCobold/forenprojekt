#include "Menu.hpp"
#include "resources/ResourceManager.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

Menu::Menu(sf::Vector2f position, sf::RenderWindow& screen, ResourceManager& resourceManager) :
m_position(position),
m_screen(screen),
m_resourceManager(resourceManager)
{
}

Menu::~Menu()
{}