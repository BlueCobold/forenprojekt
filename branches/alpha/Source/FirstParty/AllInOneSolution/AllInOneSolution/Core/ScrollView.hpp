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
    ScrollView(const sf::Vector2u& levelsize, const sf::Vector2f& viewsize);

    /*
        Adjust the view of the given render window. 
        scrollvec is the ball's position.
    */
    void adjustView(const sf::Vector2f& scrollvec, sf::RenderTarget& window);

    void setLevelSize(const sf::Vector2f& v);
    void setZoomFactor(const float factor);

    sf::Vector2f toGlobalCoords(const sf::Vector2f& v);

    float getGlobalRightCorner();
    float getGlobalTopCorner();
    float getGloablLeftCorner();
    float getGlobalBottomCorner();

private:
    float m_zoomFactor;
    sf::Vector2f m_viewSize;  // view dimension
    sf::Vector2f m_levelSize; // level dimension

    sf::Vector2f m_viewCenter;
};

#endif