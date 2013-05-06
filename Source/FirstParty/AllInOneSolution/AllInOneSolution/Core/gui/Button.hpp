#pragma once

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "../rendering/Drawable.hpp"
#include "../gui/BitmapFont.hpp"
#include "LineLabel.hpp"
#include "../resources/ResourceManager.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <functional>

/// this class represent a Button
class Button
{
public:

    struct ButtonData 
    {
        sf::Sprite m_sprite;
        BitmapFont* m_font;
        sf::Vector2f m_spriteOffset;
        sf::Vector2f m_fontOffset;
        bool m_bound;
    };

private:

    sf::Vector2f m_position;
    sf::Vector2f m_offset;
    sf::Vector2f m_textOffset;
    sf::Vector2i m_size;

    LineLabel m_buttonLabel;
    sf::Sprite m_buttonSprite;

    int m_id;

    ButtonData m_idleData;
    ButtonData m_hoverData;
    ButtonData m_pressedData;

    std::function<void (const Button& sender)> m_callback;

public:

    Button(const sf::Vector2f& position,
           const std::string& text,
           BitmapFont* font,
           const sf::Sprite& buttonIdlePicture,
           const int id,
           const sf::Vector2f& offset,
           const sf::Vector2f& textOffset = sf::Vector2f(0, 0));

    virtual ~Button()
    { }

    virtual void update(const sf::RenderWindow& screen);

    virtual void draw(const DrawParameter& params);
    
    void bindHover(const sf::Sprite& sprite,
                   BitmapFont* font,
                   const sf::Vector2f& spriteOffset,
                   const sf::Vector2f& fontOffset);

    void bindPressed(const sf::Sprite& sprite,
                     BitmapFont* font,
                     const sf::Vector2f& spriteOffset,
                     const sf::Vector2f& fontOffset);

    void registerOnPressed(std::function<void (const Button& sender)> callback);

    void setPosition(const sf::Vector2f& position);

    const sf::Vector2i& getSize() const;

    int getId() const;
};

#endif // BUTTON_HPP
