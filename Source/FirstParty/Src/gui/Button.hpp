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
class Button : public MenuElement, public SizedElement
{
    ButtonStyle m_style;
    ToolTip m_toolTip;
    bool m_showToolTip;
    bool m_isTriggering;
    bool m_keepAspectRatio;
    sf::Vector2f m_scale;

    ButtonStateStyle* m_currentStyle;

    bool m_playHoverSound;
    bool m_playPressedSound;

    std::function<void (const Button& sender)> m_callback;

    std::unique_ptr<MenuElement> onClone() const override;
    void onDrawElement(const DrawParameter& params) override;
    void updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0)) override;
    void onDrawAdditionalForeground(const DrawParameter& params) override;
    void layoutUpdated(const sf::Vector2f& screenSize) override;

public:

    Button(int id, 
           ButtonStyle style,
           const ScreenLocation& position,
           const ScreenSize& size,
           bool triggers = true);

    void registerOnPressed(std::function<void (const Button& sender)> callback);

    void setToolTip(const ToolTip& toolTip);
    void setToolTipText(const std::string& text, const std::string& replacement = "");
    void setScale(const sf::Vector2f& scale, bool keepAspectRatio);

    void changeIdleSprite(const Sprite& sprite);
    void changeHoverSprite(const Sprite& sprite);
    void changePressedSprite(const Sprite& sprite);

private:
    void setStyle(ButtonStateStyle& style, double time, const sf::Vector2f& screenSize);
};

#endif // BUTTON_HPP
