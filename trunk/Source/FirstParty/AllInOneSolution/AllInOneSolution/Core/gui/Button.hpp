#pragma once

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "ButtonStyle.hpp"
#include "LineLabel.hpp"
#include "MenuElement.hpp"
#include "ToolTip.hpp"
#include "../gui/BitmapFont.hpp"
#include "../model/SoundObject.hpp"
#include "../resources/ResourceManager.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include <functional>

/// this class represent a Button
class Button : public MenuElement
{
private:

    ButtonStyle m_style;
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

    virtual void update(const sf::RenderWindow& screen, const float time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0)) override;

    virtual void draw(const DrawParameter& params) override;

    void registerOnPressed(std::function<void (const Button& sender)> callback);

    const sf::Vector2i& getSize() const;
    
    void setToolTip(const ToolTip& toolTip);

    void setToolTipText(const std::string& text);

    virtual void drawAdditionalForeground(const DrawParameter& params) override;

    void changeIdleSprite(const sf::Sprite& sprite);
    void changeHoverSprite(const sf::Sprite& sprite);
    void changePressedSprite(const sf::Sprite& sprite);

protected:

    virtual void onPositionChanged() override;
};

#endif // BUTTON_HPP
