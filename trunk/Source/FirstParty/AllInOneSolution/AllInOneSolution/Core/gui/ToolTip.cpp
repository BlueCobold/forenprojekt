#include "ToolTip.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

ToolTip::ToolTip(const std::string& text,
                 BitmapFont* font,
                 const sf::Vector2f& textOffset,
                 const sf::Vector2f& offset,
                 const std::unordered_map<int, sf::Sprite>& background) :
    m_background(background),
    m_offset(offset),
    m_textOffset(textOffset),
    m_position(sf::Vector2f(0, 0)),
    m_scalefactorHorizontal(1.f),
    m_scalefactorVertical(1.f),
    m_lines(-1),
    m_font(font),
    m_longestLine(0),
    m_width(0),
    m_height(0)
{
    if(text != "")
    {
        setLines(text);
        m_longestLine = findLongestLine();
        m_width = m_label.find(m_longestLine)->second.getWidth();
        m_height = static_cast<float>(m_label.find(m_longestLine)->second.getFontSize() * m_lines);
        m_scalefactorHorizontal = m_width / m_background.find(ToolTip::BottomCenter)->second.getTextureRect().width;
        m_scalefactorVertical = m_height * m_lines / m_background.find(ToolTip::MiddleLeft)->second.getTextureRect().height;;
        stretchBackground();
    }
}

ToolTip::ToolTip() :
    m_offset(sf::Vector2f(0, 0)),
    m_textOffset(sf::Vector2f(0, 0)),
    m_position(sf::Vector2f(0, 0)),
    m_scalefactorHorizontal(1.f),
    m_scalefactorVertical(1.f),
    m_lines(-1),
    m_font(nullptr),
    m_longestLine(0),
    m_width(0),
    m_height(0)
{
}

void ToolTip::draw(const DrawParameter& params)
{
    if(m_label.size() == 0)
        return;

    if(m_lines != -1)
    {
        for(auto it = begin(m_background); it != end(m_background); ++it)
            params.getTarget().draw(it->second);
        for(auto it = begin(m_label); it != end(m_label); ++it)
            it->second.draw(params);
    }
}

void ToolTip::setPosition(const sf::Vector2f& position)
{
    if(m_lines != -1)
    {
        for(auto it = begin(m_label); it != end(m_label); ++it)
            it->second.setPosition(position.x + m_offset.x + m_textOffset.x,
                                   position.y + m_textOffset.y + m_offset.y + it->first * m_height / m_lines);

        auto height = m_background[TopLeft].getTextureRect().height;
        auto width = m_background[TopLeft].getTextureRect().width;

        m_background[TopLeft].setPosition(position.x - m_width / 2.f - width + m_offset.x,
                                         position.y - height + m_offset.y);

        m_background[TopCenter].setPosition(position.x - m_width / 2.f + m_offset.x,
                                           position.y - height + m_offset.y);

        m_background[TopRight].setPosition(position.x + m_width / 2.f + m_offset.x,
                                          position.y - height + m_offset.y);

        m_background[MiddleLeft].setPosition(position.x - m_width / 2.f - width + m_offset.x,
                                            position.y + m_offset.y);

        m_background[MiddleCenter].setPosition(position.x - m_width / 2.f + m_offset.x,
                                              position.y + m_offset.y);

        m_background[MiddleRight].setPosition(position.x + m_width / 2.f + m_offset.x,
                                             position.y + m_offset.y);

        m_background[BottomLeft].setPosition(position.x - m_width / 2.f - width + m_offset.x,
                                            position.y + m_height + m_offset.y);

        m_background[BottomCenter].setPosition(position.x - m_width / 2.f + m_offset.x,
                                              position.y + m_height + m_offset.y);

        m_background[BottomRight].setPosition(position.x + m_width / 2.f + m_offset.x,
                                             position.y + m_height + m_offset.y);
    }
}

void ToolTip::setText(const std::string& text)
{
    if(text != "")
    {
        setLines(text);
        m_longestLine = findLongestLine();
        m_width = m_label.find(m_longestLine)->second.getWidth();
        m_height = static_cast<float>(m_label.find(m_longestLine)->second.getFontSize() * m_lines);
        m_scalefactorHorizontal = m_width / m_background.find(ToolTip::BottomCenter)->second.getTextureRect().width;
        m_scalefactorVertical = m_height / m_background.find(ToolTip::MiddleLeft)->second.getTextureRect().height;;
        stretchBackground();
    }
    else
        m_lines = -1;
}

void ToolTip::setLines(const std::string& text)
{
    if(text == "")
        return;

    m_lines = 0;

    m_label.clear();

    std::string tokens = text.c_str();
    std::string token;

    // multilines
    for(int pos = tokens.find("\\r\\n"); pos != std::string::npos; pos = tokens.find("\\r\\n"))
    {
        token = tokens.substr(0, pos);
        tokens.erase(0, pos + 4);
        LineLabel label(token, sf::Vector2f(), sf::Vector2f(), 0, m_font, LineLabel::Centered);
        m_label[m_lines++] = label;
    }
    // single line or last multiline
    LineLabel label(tokens, sf::Vector2f(), sf::Vector2f(), 0, m_font, LineLabel::Centered);
    m_label[m_lines++] = label;
}

const int ToolTip::findLongestLine() const
{
    int longestLine = -1;
    float longestwidth = 0;

    for(auto it = begin(m_label); it != end(m_label); ++it)
    {
        if( longestwidth < it->second.getWidth())
        {
            longestwidth = it->second.getWidth();
            longestLine = it->first;
        }
    }
    return longestLine;
}

void ToolTip::stretchBackground()
{
    m_background[BottomCenter].setScale(m_scalefactorHorizontal, 1.f);
    m_background[MiddleCenter].setScale(m_scalefactorHorizontal, m_scalefactorVertical);
    m_background[TopCenter].setScale(m_scalefactorHorizontal, 1.f);
    m_background[MiddleLeft].setScale(1.f, m_scalefactorVertical);
    m_background[MiddleRight].setScale(1.f, m_scalefactorVertical);
}
