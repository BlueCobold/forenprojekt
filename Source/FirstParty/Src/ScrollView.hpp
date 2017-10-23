#pragma once

#ifndef SCROLLVIEW_INCLUDED
#define SCROLLVIEW_INCLUDED

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "rendering/DrawParameter.hpp"

class ScrollView
{
public:
    ScrollView();
    ScrollView(const sf::Vector2u& levelsize, const sf::Vector2f& viewsize);

    /*
        Adjust the view of the given render window.
        scrollvec is the ball's position.
    */
    void adjustView(const sf::Vector2f& scrollvec, const DrawParameter& window);

    void setLevelSize(const sf::Vector2f& v);
    void setZoomFactor(const float factor);

private:
    sf::Vector2f m_viewSize;
    sf::Vector2f m_levelSize;
    float m_zoomFactor;
};

#endif
