#ifndef SCROLLVIEW_INCLUDED
#define SCROLLVIEW_INCLUDED

#pragma once

#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Entity.hpp"

class ScrollView
{
public:
	ScrollView(const sf::Vector2u& levelsize, const sf::Vector2u& viewsize);

	/*
		Adjust the view of the given render window. 
		scrollvec is the ball's position.
	*/
	void adjustView(const sf::Vector2f& scrollvec, sf::RenderWindow& window);

private:
	const sf::Vector2u m_viewSize;  // view dimension
	sf::Vector2f m_levelSize; // level dimension
};

#endif