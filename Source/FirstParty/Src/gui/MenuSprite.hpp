#pragma once

#ifndef MENUSPRITE_HPP
#define MENUSPRITE_HPP

#include "MenuElement.hpp"
#include "ToolTip.hpp"

#include <SFML/Graphics/Sprite.hpp>

class MenuSprite : public MenuElement
{
public:

    MenuSprite(const sf::Sprite& sprite, const sf::Vector2f& position, const sf::Vector2f& offset, const int id);

    void setToolTip(const ToolTip& toolTip);
    void setToolTipText(const std::string& text, const std::string& replacement = "");
    void setTextureRect(const sf::IntRect& textureRect);
    void setTexture(const sf::Texture& texture);

    void update(const sf::RenderWindow& screen, const float time, const sf::Vector2i& offset = sf::Vector2i(0, 0)) override;

    void drawAdditionalForeground(const DrawParameter& params) override;

    void setLanguage(const std::string& language) override;

protected:
    void updateLayout(const sf::Vector2f& screenSize) override;

private:
    void doDraw(const DrawParameter& params) override;

    sf::Sprite m_sprite;
    ToolTip m_toolTip;
    bool m_showToolTip;

    std::unique_ptr<MenuElement> doClone() const override;
};

#endif // MENUSPRITE_HPP
