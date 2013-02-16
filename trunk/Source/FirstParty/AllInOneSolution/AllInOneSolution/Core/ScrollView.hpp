#ifndef SCROLLVIEW_INCLUDED
#define SCROLLVIEW_INCLUDED

#pragma once

#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "model/Entity.hpp"

class ScrollView
{
public:
	ScrollView();
	ScrollView(const sf::Vector2u& levelsize, const sf::Vector2u& viewsize);

	/*
		Adjust the view of the given render window. 
		scrollvec is the ball's position.
	*/
    void adjustView(const sf::Vector2f& scrollvec, sf::RenderTarget& window);

	void setViewSize(const sf::Vector2u& v);
	void setLevelSize(const sf::Vector2u& v);

    sf::Vector2f toGlobalCoords(const sf::Vector2u& v);

    unsigned int getGlobalRightCorner();
    unsigned int getGlobalTopCorner();
    unsigned int getGloablLeftCorner();
    unsigned int getGlobalBottomCorner();

private:
	sf::Vector2u m_viewSize;  // view dimension
	sf::Vector2u m_levelSize; // level dimension

    sf::Vector2f m_viewCenter;
};

#endif