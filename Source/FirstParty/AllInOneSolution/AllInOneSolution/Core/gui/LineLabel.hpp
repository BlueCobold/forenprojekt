#ifndef LINELABEL_HPP
#define LINELABEL_HPP

#pragma once

#include <SFML/System/Vector2.hpp>

#include "../rendering/Drawable.hpp"
#include "BitmapFont.hpp"

/// This class will be used to draw a single line label 
/// with bitmapfonts
class LineLabel : public Drawable
{
public:
    LineLabel();
    LineLabel(const std::string& text, const sf::Vector2f& position, const float rotation, BitmapFont* font);
    LineLabel(const std::string& text, const float x, const float y, float rotation, BitmapFont* font);

    void setText(const std::string& text);
    std::string getText() const;

    void setPosition(const sf::Vector2f position);
    void setPosition(const float x, const float y);

    sf::Vector2f getPosition() const;

    void setRotation(const float angle);
    float getRotation() const;

    void setBitmapFont(BitmapFont* font);

    void draw(const DrawParameter& params);

private:
    std::string m_text;
    sf::Vector2f m_position;
    float m_rotation;
    BitmapFont* m_font;

};

#endif // LINELABEL_HPP
