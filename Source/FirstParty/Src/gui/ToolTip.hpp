#pragma once

#ifndef TOOLTIP_HPP
#define TOOLTIP_HPP

#include "LineLabel.hpp"
#include <unordered_map>

namespace sf
{
    class RenderWindow;
};

class ToolTip : public Drawable
{
private:
    std::unordered_map<int, sf::Sprite> m_background;
    std::unordered_map<int, LineLabel> m_label;
    sf::Vector2f m_offset;
    sf::Vector2f m_textOffset;
    sf::Vector2f m_position;
    float m_scalefactorHorizontal;
    float m_scalefactorVertical;
    int m_lines;
    const BitmapFont* m_font;
    int m_longestLine;
    float m_width;
    float m_height;
    std::string m_textKey;
    std::string m_lastTranslation;
    std::string m_lastReplacement;

    void setLines(const std::string& text, const std::string& replacement = "");
    const int findLongestLine() const;
    void stretchBackground();
    sf::Vector2f calculateNeededOffset(const sf::Vector2f& position, const sf::RenderWindow& screen);

    void doDraw(const DrawParameter& params) override;

public:
    enum BackgroundId {TopLeft = 1,
                      TopCenter = 2,
                      TopRight = 3,
                      MiddleLeft = 4,
                      MiddleCenter = 5,
                      MiddleRight = 6,
                      BottomLeft = 7,
                      BottomCenter = 8,
                      BottomRight = 9};

    ToolTip(const std::string& text,
            const BitmapFont* font,
            const sf::Vector2f& textOffset,
            const sf::Vector2f& offset,
            const std::unordered_map<int, sf::Sprite>& background);
    ToolTip();

    void setPosition(const sf::Vector2f& position, const sf::RenderWindow& screen);

    void setText(const std::string& text, const std::string& replacement = "");
    void update();
};

#endif // TOOLTIP_HPP
