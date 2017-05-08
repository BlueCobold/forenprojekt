#pragma once

#ifndef BORDER_HPP
#define BORDER_HPP

#include "BorderStyle.hpp"
#include "MenuElement.hpp"
#include "ScreenLocation.hpp"
#include "ScreenSize.hpp"
#include "../rendering/Sprite.hpp"

#include <SFML/Graphics/Rect.hpp>

#include <array>
#include <vector>
#include <unordered_map>
#include <utility>

namespace sf
{
    class RenderWindow;
};

class Border : public MenuElement, public SizedElement
{
public:
    Border(int id,
           const ScreenLocation& position,
           const ScreenSize& size,
           const BorderStyle& style);
    
    void setScale(const sf::Vector2f& scale, bool keepAspectRatio);

private:
    BorderStyle m_style;
    bool m_keepAspectRatio;
    sf::Vector2f m_scale;
    bool m_hasDecos;

    void onDrawElement(const DrawParameter& params) override;
    std::unique_ptr<MenuElement> onClone() const override;
    void updateDeco(BorderStyle::DecoId id, float x, float y);
    void updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0)) override;
};

#endif // BORDER_HPP
