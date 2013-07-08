#pragma once

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "../rendering/Drawable.hpp"
#include "../gui/BitmapFont.hpp"
#include "LineLabel.hpp"
#include "../resources/ResourceManager.hpp"
#include "ButtonStyle.hpp"
#include "../model/SoundObject.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <functional>

/// this class represent a Button
class Button
{
private:

    int m_id;
    ButtonStyle m_style;
    SoundObject m_sound;
    sf::Vector2f m_position;
    sf::Vector2f m_offset;
    sf::Vector2i m_size;

    LineLabel* m_label;
    sf::Sprite* m_sprite;

    bool m_playHoverSound;

    std::function<void (const Button& sender)> m_callback;

public:

    Button(int id, ButtonStyle style, const SoundObject& sound, const sf::Vector2f& position, const sf::Vector2f& offset);

    virtual ~Button()
    { }

    virtual void update(const sf::RenderWindow& screen);

    virtual void draw(const DrawParameter& params);

    void registerOnPressed(std::function<void (const Button& sender)> callback);

    void setPosition(const sf::Vector2f& position);

    const sf::Vector2i& getSize() const;

    int getId() const;
};

#endif // BUTTON_HPP
