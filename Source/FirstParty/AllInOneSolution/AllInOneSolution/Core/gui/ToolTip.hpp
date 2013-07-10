#pragma once

#ifndef TOOLTIP_HPP
#define TOOLTIP_HPP

#include "LineLabel.hpp"

namespace sf
{
    class RenderWindow;
};

class ToolTip : public Drawable
{
private:
    LineLabel m_label;
    sf::Vector2f m_offset;
    sf::Vector2f m_position;
    sf::Sprite m_leftTexture;
    sf::Sprite m_centerTexture;
    sf::Sprite m_rightTexture;
    float m_scalefactor;

public:
     ToolTip(const std::string& text, BitmapFont* font, const sf::Vector2f& textOffset, const sf::Vector2f& offset,
             const sf::Sprite& leftTexture, const sf::Sprite& centerTexture, const sf::Sprite& rightTexture);
     ToolTip();
     virtual void draw(const DrawParameter& params);

     void setPosition(const sf::Vector2f& position);
};

#endif // TOOLTIP_HPP