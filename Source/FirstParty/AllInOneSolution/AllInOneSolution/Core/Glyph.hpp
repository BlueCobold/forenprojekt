#ifndef GLYPH_HPP
#define GLYPH_HPP

#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <iostream>

/// This class discribes a single letter from a bitmap font
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

    sf::IntRect getTextureRect() {return m_textureRect;}

    void setTextureRect(sf::IntRect textureRect) {m_textureRect = textureRect;}

    void setTextureRect(int top, int left, int width, int height) 
    {
        m_textureRect.top = top;
        m_textureRect.left = left;
        m_textureRect.width = width;
        m_textureRect.height = height;
    }
};


#endif