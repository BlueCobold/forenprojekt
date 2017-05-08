#ifndef LINELABEL_HPP
#define LINELABEL_HPP

#pragma once

#include "BitmapFont.hpp"
#include "MenuElement.hpp"
#include "../animation/Interpolation.hpp"
#include "../rendering/Drawable.hpp"

#include <SFML/System/Vector2.hpp>

#include <memory>

/// This class will be used to draw a single line label 
/// with bitmapfonts
class LineLabel : public MenuElement
{
public:

    enum Alignment
    {
        Left = 0,
        Centered = 1,
        Right = 2
    };

    LineLabel();
    LineLabel(const std::string& text,
        const ScreenLocation& position,
        const float rotation,
        const BitmapFont* font,
        const Alignment alignment = Left,
        int id = -1);

    void setText(const std::string& text, const std::string& key = "");
    std::string getText() const;
    float getWidth() const;

    void setAlignment(const Alignment alignment);

    void attachPositionProgress(const Interpolation& x, const Interpolation& y);
    void attachAlphaProgress(const Interpolation& alpha);
    void updateProgress(const double time);
    bool anyProgressRunning();
    bool allProgressesFinished();

    void setRotation(const float angle);
    float getRotation() const;

    void setBitmapFont(BitmapFont* font);

    unsigned int getFontSize();

    LineLabel(const std::string& text,
        const ScreenLocation& position,
        const float rotation,
        const MenuElementType::Type type,
        const BitmapFont* font,
        const Alignment alignment = Left,
        int id = -1);

    Alignment getAlignment() const;
    const BitmapFont* getFont() const;
    
protected:
    void onDrawElement(const DrawParameter& params) override;
    void updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0)) override;
    void layoutUpdated(const sf::Vector2f& screenSize) override;

private:
    std::unique_ptr<MenuElement> onClone() const override;

    void rebuild();

    float m_width;
    std::string m_text;
    std::string m_textKey;
    float m_rotation;
    const BitmapFont* m_font;
    std::vector<BitmapFont::Glyph> m_glyphs;
    Alignment m_alignment;

    sf::Vector2f m_progressPosition;
    Interpolation m_xPosChange;
    Interpolation m_yPosChange;
    Interpolation m_alphaChange;
};

#endif // LINELABEL_HPP