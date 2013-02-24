#ifndef LINELABEL_HPP
#define LINELABEL_HPP

#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "../rendering/Drawable.hpp"
#include "BitmapFont.hpp"

enum Aligend {RightAligend, LeftAligend, CenterAligend};

/// This class will be used to draw a single line label 
/// with bitmapfonts
class LineLabel : public Drawable
{
public:
    LineLabel();
    LineLabel(const std::string& text, const sf::Vector2f& position, const float rotation, BitmapFont* font);
    LineLabel(const std::string& text, const float x, const float y, float rotation, BitmapFont* font);

    virtual void setText(const std::string& text);
    std::string getText() const;

    virtual void setPosition(const sf::Vector2f position);
    
    void setPosition(const float x, const float y);
    sf::Vector2f getPosition() const;

    virtual void setRotation(const float angle);
    float getRotation() const;

    void setBitmapFont(BitmapFont& font);

    virtual void draw(const DrawParameter& params);

    virtual void update();

    int getLength();

    void setAligend(Aligend value);

protected:

    std::string m_text;

    BitmapFont* m_font;

    sf::RenderTexture m_texture;

    Aligend m_aligend;

    //sf::Sprite m_sprite;

    sf::Vector2f m_position;

    float m_rotation;
 
};

#endif // LINELABEL_HPP
