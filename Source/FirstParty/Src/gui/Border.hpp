#pragma once

#ifndef BORDER_HPP
#define BORDER_HPP

#include "MenuElement.hpp"
#include "ScreenLocation.hpp"
#include "ScreenSize.hpp"
#include "../rendering/Sprite.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <array>
#include <vector>
#include <unordered_map>
#include <utility>

namespace sf
{
    class RenderWindow;
};

class Border : public MenuElement
{
public:
    enum BackgroundId
    {
        TopLeft = 1,
        TopCenter = 2,
        TopRight = 3,
        MiddleLeft = 4,
        MiddleRight = 5,
        BottomLeft = 6,
        BottomCenter = 7,
        BottomRight = 8,
        Background = 9
    };

    enum DecoId
    {
        Top = 0,
        Right = 1,
        Left = 2,
        Bottom = 3
    };

    Border(int id,
           ScreenLocation position,
           ScreenSize size,
           std::unordered_map<BackgroundId, Sprite> backgrounds,
           std::array<std::vector<std::pair<Sprite, sf::Vector2f>>, 4> decos
               = std::array<std::vector<std::pair<Sprite, sf::Vector2f>>, 4>());
    
    void setScale(const sf::Vector2f& scale, bool keepAspectRatio);

private:
    std::unordered_map<BackgroundId, Sprite> m_backgrounds;
    std::array<std::vector<std::pair<Sprite, sf::Vector2f>>, 4> m_decos;
    ScreenSize m_size;
    bool m_hasDecos;
    bool m_keepAspectRatio;
    sf::Vector2f m_scale;

    void doDraw(const DrawParameter& params) override;
    std::unique_ptr<MenuElement> doClone() const override;
    void updateDeco(DecoId id, float x, float y);
    void updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0)) override;
};

#endif // BORDER_HPP
