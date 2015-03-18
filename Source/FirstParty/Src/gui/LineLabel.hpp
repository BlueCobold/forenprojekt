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
        Left,
        Centered,
        Right
    };

    LineLabel();
    LineLabel(const std::string& text,
        const sf::Vector2f& position,
        const sf::Vector2f& offset,
        const float rotation,
        BitmapFont* font,
        const Alignment alignment = Left,
        int id = -1);

    virtual void draw(const DrawParameter& params) override;

    void setText(const std::string& text);
    std::string getText() const;
    float getWidth() const;

    void setAlignment(const Alignment alignment);

    void attachPositionProgress(const Interpolation& x, const Interpolation& y);
    void attachAlphaProgress(const Interpolation& alpha);
    void updateProgress(const float time);
    bool anyProgressRunning();
    bool allProgressesFinished();

    void setRotation(const float angle);
    float getRotation() const;

    void setBitmapFont(BitmapFont* font);

    unsigned int getFontSize();

protected:

    virtual void onPositionChanged() override;

private:

    void rebuild();

    float m_width;
    std::string m_text;
    float m_rotation;
    BitmapFont* m_font;
    std::vector<BitmapFont::Glyph> m_glyphs;
    Alignment m_alignment;

    sf::Vector2f m_progressPosition;
    Interpolation m_xPosChange;
    Interpolation m_yPosChange;
    Interpolation m_alphaChange;
};

#endif // LINELABEL_HPP