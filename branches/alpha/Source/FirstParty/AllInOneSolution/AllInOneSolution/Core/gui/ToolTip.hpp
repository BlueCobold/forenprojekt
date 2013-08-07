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
    std::unordered_map<int, LineLabel> m_label;
    BitmapFont* m_font;
    sf::Vector2f m_offset;
    sf::Vector2f m_position;
    sf::Vector2f m_textOffset;
    std::unordered_map<int, sf::Sprite> m_backround;
    float m_scalefactorHorizontal;
    float m_scalefactorVertical;
    int m_lines;
    int m_longestLine;
    float m_width;
    float m_height;

    void setLines(const std::string& text);
    const int findLongestLine() const;
    void strechBackround();

public:
    enum BackroundId {TopLeft = 1,
                      TopCenter = 2,
                      TopRight = 3,
                      MiddleLeft = 4,
                      MiddleCenter = 5,
                      MiddleRight = 6,
                      BottomLeft = 7,
                      BottomCenter = 8,
                      BottomRight = 9};

    ToolTip(const std::string& text,
            BitmapFont* font, 
            const sf::Vector2f& textOffset, 
            const sf::Vector2f& offset,
            const std::unordered_map<int, sf::Sprite>& backround);
    ToolTip();

    virtual void draw(const DrawParameter& params) override;

    void setPosition(const sf::Vector2f& position);

    void setText(const std::string& text);
};

#endif // TOOLTIP_HPP
