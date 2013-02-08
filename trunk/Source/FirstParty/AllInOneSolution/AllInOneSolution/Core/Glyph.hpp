#ifndef GLYPH_HPP
#define GLYPH_HPP

#include "SFML\Graphics\Rect.hpp"
#include <iostream>

class Glyph
{
    
private:
    sf::IntRect m_textureRect;

public:

    Glyph(int top, int left, int width, int height)
    {
        m_textureRect.top = top;
        m_textureRect.left = left;
        m_textureRect.width = width;
        m_textureRect.height = height;
    }
    Glyph(sf::IntRect textureRect) { m_textureRect = textureRect;}

    sf::IntRect gettextureRect() {return m_textureRect;}

    void settextureRect(sf::IntRect textureRect) {m_textureRect = textureRect;}

    void settextureRect(int top, int left, int width, int height) 
    {
        m_textureRect.top = top;
        m_textureRect.left = left;
        m_textureRect.width = width;
        m_textureRect.height = height;
    }
};


#endif