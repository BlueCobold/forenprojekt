#include "Background.hpp"
#include <SFML/Graphics/RenderTarget.hpp>


Background::Background()
{
}


Background::~Background()
{
}

void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite);
}