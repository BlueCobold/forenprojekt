#pragma once

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "../rendering/Drawable.hpp"
#include "../gui/BitmapFont.hpp"
#include "LineLabel.hpp"
#include "../resources/ResourceManager.hpp"
#include "ButtonStyle.hpp"
#include "../model/SoundObject.hpp"
#include "ToolTip.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <functional>

/// this class represent a Button
class Button : public Drawable
{
private:

    int m_id;
    ButtonStyle m_style;
    sf::Vector2f m_position;
    sf::Vector2f m_offset;
    sf::Vector2i m_size;
    ToolTip m_toolTip;
    bool m_showToolTip;

    LineLabel* m_label;
    sf::Sprite* m_sprite;

    bool m_playHoverSound;
    bool m_playPressedSound;

    std::function<void (const Button& sender)> m_callback;

public:

    Button(int id, ButtonStyle style, const sf::Vector2f& position, const sf::Vector2f& offset);

    virtual ~Button()
    { }

    virtual void update(const sf::RenderWindow& screen);

    virtual void draw(const DrawParameter& params) override;

    void registerOnPressed(std::function<void (const Button& sender)> callback);

    void setPosition(const sf::Vector2f& position);

    const sf::Vector2i& getSize() const;

    int getId() const;

    void setToolTip(const ToolTip& toolTip);

    void setToolTipText(const std::string& text);

    virtual void drawAdditionalForeground(const DrawParameter& params);
};

#endif // BUTTON_HPP
