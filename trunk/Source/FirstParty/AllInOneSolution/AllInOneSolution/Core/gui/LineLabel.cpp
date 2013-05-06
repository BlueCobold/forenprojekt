#include "LineLabel.hpp"

LineLabel::LineLabel()
    : m_text(""),
    m_position(sf::Vector2f(0,0)),
    m_progressPosition(0, 0),
    m_rotation(0),
    m_font(nullptr)
{
    rebuild();
}

LineLabel::LineLabel(const std::string& text,
                    const sf::Vector2f& position,
                    const float rotation,
                    BitmapFont* font,
                    Alignment alignment)
    : m_text(text),
    m_position(position),
    m_progressPosition(0, 0),
    m_rotation(rotation),
    m_font(font),
    m_alignment(alignment),
    m_offset(sf::Vector2f(0,0))
{
    rebuild();
}

void LineLabel::draw(const DrawParameter& params)
{
    for(auto it = begin(m_glyphs); it != end(m_glyphs); it++) 
    {
        auto glyph = (*it);
        glyph.setPosition(
            glyph.getPosition().x + m_progressPosition.x,
            glyph.getPosition().y + m_progressPosition.y);

        if(m_alphaChange.isStarted())
            glyph.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * m_alphaChange.getCurrentValue())));
        params.getTarget().draw(glyph);
    }
}

void LineLabel::setText(const std::string& text)
{
    if(text != m_text)
    {
        m_text = text;
        rebuild();
    }
}

std::string LineLabel::getText() const
{
    return m_text;
}

void LineLabel::setPosition(const sf::Vector2f& position)
{
    if(position != m_position)
    {
        m_position = position;
        rebuild();
    }
}

void LineLabel::setOffset(const sf::Vector2f& offset)
{
    if(offset != m_offset)
    {
        m_offset = offset;
        rebuild();
    }
}

void LineLabel::rebuild()
{
    m_glyphs.clear();

    float shift = 0;
    if(m_alignment != Left)
    {
        float width = 0;
        for(auto it = begin(m_text); it != end(m_text); it++)
            width += m_font->getGlyph(*it).getSpacing();
        if(m_alignment == Right)
            shift -= width;
        else
            shift -= width/2;
    }

    float xOffset = 0;
    for(auto it = begin(m_text); it != end(m_text); it++)
    {
        m_glyphs.push_back(m_font->getGlyph(*it));
        m_glyphs.back().setPosition(shift + m_position.x + xOffset + m_offset.x, m_position.y + m_offset.y);
        m_glyphs.back().setRotation(m_rotation);

        xOffset += m_font->getGlyph(*it).getSpacing();
    }
    m_width = xOffset;
}

void LineLabel::setPosition(const float x, const float y)
{
    if(m_position != sf::Vector2f(x, y))
    {
        m_position = sf::Vector2f(x, y);
        rebuild();
    }
}

void LineLabel::setAlignment(const Alignment alignment)
{
    if(m_alignment != alignment)
    {
        m_alignment = alignment;
        rebuild();
    }
}

const sf::Vector2f LineLabel::getPosition() const
{
    return m_position + m_progressPosition;
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

void LineLabel::updateProgress(const float time)
{
    sf::Vector2f pos(0, 0);
    if(m_xPosChange.isStarted() && !m_xPosChange.isFinished())
    {
        m_xPosChange.update(time);
        pos.x = floorf(m_xPosChange.getCurrentValue());
    }
    if(m_yPosChange.isStarted() && !m_yPosChange.isFinished())
    {
        m_yPosChange.update(time);
        pos.y = floorf(m_yPosChange.getCurrentValue());
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