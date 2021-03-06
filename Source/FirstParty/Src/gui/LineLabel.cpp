#include "LineLabel.hpp"
#include "../Utility.hpp"

#include <cmath>

#include <SFML/System/Mutex.hpp>

LineLabel::LineLabel() :
    MenuElement(-1, MenuElementType::Label, sf::Vector2f(0, 0), sf::Vector2f(0, 0)),
    m_text(""),
    m_textKey(""),
    m_rotation(0),
    m_font(nullptr),
    m_progressPosition(0, 0)
{
    rebuild();
}

LineLabel::LineLabel(const std::string& text,
                     const ScreenLocation& position,
                     const ScreenScale& scale,
                     const float rotation,
                     const BitmapFont* font,
                     Alignment alignment,
                     int id) :
    MenuElement(id, MenuElementType::Label, position),
    m_scale(scale),
    m_textKey(text),
    m_rotation(rotation),
    m_font(font),
    m_alignment(alignment),
    m_progressPosition(0, 0)
{
    if(m_textKey != "")
        m_text = utility::translateKey(m_textKey);
    else
        m_text = "";

    m_text = utility::replaceAll(m_text, "\\r\\n", "\n");
    rebuild();
}

LineLabel::LineLabel(const std::string& text,
                     const ScreenLocation& position,
                     const ScreenScale& scale,
                     const float rotation,
                     const MenuElementType::Type type,
                     const BitmapFont* font,
                     const Alignment alignment,
                     int id) :
    MenuElement(id, type, position),
    m_scale(scale),
    m_textKey(text),
    m_rotation(rotation),
    m_font(font),
    m_alignment(alignment),
    m_progressPosition(0, 0)
{
    if(m_textKey != "")
        m_text = utility::translateKey(m_textKey);
    else
        m_text = "";

    m_text = utility::replaceAll(m_text, "\\r\\n", "\n");
    rebuild();
}

std::unique_ptr<MenuElement> LineLabel::onClone() const
{
    return std::unique_ptr<MenuElement>(new LineLabel(m_textKey, getPosition(), m_scale, m_rotation, getType(), m_font, m_alignment, getId()));
}

namespace
{
    sf::Mutex mutex;
};

void LineLabel::onDrawElement(const DrawParameter& params)
{
    mutex.lock();
    auto glyphs(m_glyphs);
    mutex.unlock();
    auto currentPosition = getCurrentPosition();
    for(auto it = begin(glyphs); it != end(glyphs); it++)
    {
        auto glyph = (*it);
        glyph.setPosition(
            floorf(currentPosition.x + glyph.getPosition().x + m_progressPosition.x),
            floorf(currentPosition.y + glyph.getPosition().y + m_progressPosition.y));

        if(m_alphaChange.isStarted())
            glyph.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * m_alphaChange.getCurrentValue())));
        params.getTarget().draw(glyph);
    }
}

void LineLabel::setText(const std::string& text, const std::string& key)
{
    auto t = utility::replaceAll(text, "\\r\\n", "\n");
    if(t != m_text)
    {
        m_text = t;
        m_textKey = key;
        rebuild();
    }
}

std::string LineLabel::getText() const
{
    return m_text;
}

void LineLabel::rebuild()
{
    std::vector<BitmapFont::Glyph> newGlyphs;

    float shift = 0;
    float scale = m_scale.getCurrentScale().x;
    if(m_alignment != Left)
    {
        float width = 0;
        float maxWidth = 0;
        for(auto it = begin(m_text); it != end(m_text); it++)
        {
            if(*it == '\n')
            {
                maxWidth = std::max(width, maxWidth);
                width = 0;
                continue;
            }
            width += m_font->getGlyph(*it).getSpacing();
        }
        if(m_alignment == Right)
            shift -= scale * std::max(width, maxWidth);
        else
            shift -= scale * std::max(width, maxWidth) / 2;
    }

    float xOffset = 0;
    float yOffset = 0;
    for(auto it = begin(m_text); it != end(m_text); it++)
    {
        if(*it == '\n')
        {
            xOffset = 0;
            yOffset += floorf(scale * 1.1f * m_font->getFontSize());
            continue;
        }

        auto glyph = m_font->getGlyph(*it);
        glyph.setPosition(shift + xOffset, scale * glyph.getVerticalOffset() + yOffset);
        glyph.setScale(scale, scale);
        glyph.setRotation(m_rotation);
        newGlyphs.push_back(glyph);

        xOffset += glyph.getSpacing();
    }
    m_width = xOffset;
    mutex.lock();
    m_glyphs = std::move(newGlyphs);
    mutex.unlock();
}

void LineLabel::setAlignment(const Alignment alignment)
{
    if(m_alignment != alignment)
    {
        m_alignment = alignment;
        rebuild();
    }
}

void LineLabel::setRotation(const float rotation)
{
    m_rotation = rotation;
}

float LineLabel::getRotation() const
{
    return m_rotation;
}

void LineLabel::setBitmapFont(BitmapFont* font)
{
    if(m_font != font)
    {
        m_font = font;
        rebuild();
    }
}

float LineLabel::getWidth() const
{
    return m_width;
}

void LineLabel::attachPositionProgress(const Interpolation& x, const Interpolation& y)
{
    m_xPosChange = x;
    if(!m_xPosChange.isStarted())
        m_xPosChange.start();

    m_yPosChange = y;
    if(!m_yPosChange.isStarted())
        m_yPosChange.start();
}

void LineLabel::attachAlphaProgress(const Interpolation& alpha)
{
    m_alphaChange = alpha;
    if(!m_alphaChange.isStarted())
        m_alphaChange.start();
}

void LineLabel::updateProgress(const double time)
{
    sf::Vector2f pos(0, 0);
    if(m_xPosChange.isStarted() && !m_xPosChange.isFinished())
    {
        m_xPosChange.update(time);
        pos.x = floorf(static_cast<float>(m_xPosChange.getCurrentValue()));
    }
    if(m_yPosChange.isStarted() && !m_yPosChange.isFinished())
    {
        m_yPosChange.update(time);
        pos.y = floorf(static_cast<float>(m_yPosChange.getCurrentValue()));
    }
    m_progressPosition = pos;

    if(m_alphaChange.isStarted() && !m_alphaChange.isFinished())
        m_alphaChange.update(time);
}

bool LineLabel::anyProgressRunning()
{
    return m_xPosChange.isStarted() || m_yPosChange.isStarted() || m_alphaChange.isStarted();
}

bool LineLabel::allProgressesFinished()
{
    return (m_xPosChange.isStarted() && m_xPosChange.isFinished())
        && (m_yPosChange.isStarted() && m_yPosChange.isFinished())
        && (m_alphaChange.isStarted() && m_alphaChange.isFinished());
}

unsigned int LineLabel::getFontSize()
{
    return m_font->getFontSize();
}

LineLabel::Alignment LineLabel::getAlignment() const
{
    return m_alignment;
}

const BitmapFont* LineLabel::getFont() const
{
    return m_font;
}

void LineLabel::layoutUpdated(const sf::Vector2f& screenSize)
{
    float oldscale = m_scale.getCurrentScale().x;
    m_scale.setScreenSize(screenSize);
    if(m_scale.getCurrentScale().x != oldscale)
        rebuild();
}

void LineLabel::updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset)
{
    updateLayout(sf::Vector2f(screen.getSize()));
    
    if(m_textKey != "")
    {
        auto next = utility::translateKey(m_textKey);
        if(next != m_text)
            setText(next, m_textKey);
    }
}
