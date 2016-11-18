#include "ToolTip.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>
#include "../Utility.hpp"

ToolTip::ToolTip(const std::string& text,
                 const BitmapFont* font,
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
    m_height(0),
    m_textKey(text)
{
    if(m_textKey != "")
    {
        setLines(m_textKey);
        m_longestLine = findLongestLine();
        m_width = m_label.find(m_longestLine)->second.getWidth();
        m_height = static_cast<float>(m_label.find(m_longestLine)->second.getFontSize() * m_lines);
        m_scalefactorHorizontal = m_width / m_background.find(ToolTip::BottomCenter)->second.getTextureRect().width;
        m_scalefactorVertical = m_height / m_background.find(ToolTip::MiddleLeft)->second.getTextureRect().height;
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

void ToolTip::doDraw(const DrawParameter& params)
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

void ToolTip::setPosition(const sf::Vector2f& position, const sf::RenderWindow& screen)
{
    if(m_lines != -1)
    {
        float mobileOffset = 0;
#if defined(IOS) || defined(ANDROID)
        mobileOffset = m_offset.y + m_height + 30.f;
#endif
        sf::Vector2f offset = calculateNeededOffset(position - sf::Vector2f(0, mobileOffset) , screen);

        for(auto it = begin(m_label); it != end(m_label); ++it)
        {
            it->second.setOffset(sf::Vector2f(position.x + m_offset.x + m_textOffset.x + offset.x,
                                 position.y + m_textOffset.y + m_offset.y + offset.y - mobileOffset + it->first * m_height / m_lines));

            it->second.update(screen, 0);
        }

        auto height = m_background[TopLeft].getTextureRect().height;
        auto width = m_background[TopLeft].getTextureRect().width;

        m_background[TopLeft].setPosition(position.x - static_cast<int>(m_width / 2.f) - width + m_offset.x + offset.x,
                                         position.y - height + m_offset.y + offset.y - mobileOffset);

        m_background[TopCenter].setPosition(position.x - static_cast<int>(m_width / 2.f) + m_offset.x + offset.x,
                                           position.y - height + m_offset.y + offset.y - mobileOffset);

        m_background[TopRight].setPosition(position.x + static_cast<int>(m_width / 2.f) + m_offset.x + offset.x,
                                          position.y - height + m_offset.y + offset.y - mobileOffset);

        m_background[MiddleLeft].setPosition(position.x - static_cast<int>(m_width / 2.f) - width + m_offset.x + offset.x,
                                            position.y + m_offset.y + offset.y - mobileOffset);

        m_background[MiddleCenter].setPosition(position.x - static_cast<int>(m_width / 2.f) + m_offset.x + offset.x,
                                              position.y + m_offset.y + offset.y - mobileOffset);

        m_background[MiddleRight].setPosition(position.x + static_cast<int>(m_width / 2.f) + m_offset.x + offset.x,
                                             position.y + m_offset.y + offset.y - mobileOffset);

        m_background[BottomLeft].setPosition(position.x - static_cast<int>(m_width / 2.f) - width + m_offset.x + offset.x,
                                            position.y + m_height + m_offset.y + offset.y - mobileOffset);

        m_background[BottomCenter].setPosition(position.x - static_cast<int>(m_width / 2.f) + m_offset.x + offset.x,
                                              position.y + m_height + m_offset.y + offset.y - mobileOffset);

        m_background[BottomRight].setPosition(position.x + static_cast<int>(m_width / 2.f) + m_offset.x + offset.x,
                                             position.y + m_height + m_offset.y + offset.y - mobileOffset);
    }
}

void ToolTip::setText(const std::string& text, const std::string& replacement)
{
    m_textKey = text;
    if(m_textKey != "")
    {
        setLines(m_textKey, replacement);
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

void ToolTip::setLines(const std::string& text, const std::string& replacement)
{
    if(text == "")
        return;

    m_lines = 0;
    m_label.clear();

    m_lastTranslation = utility::translateKey(text);
    m_lastReplacement = replacement;
    std::string tokens = m_lastTranslation.c_str();

    std::string token;
    // multilines
    for(auto pos = tokens.find("\\r\\n"); pos != std::string::npos; pos = tokens.find("\\r\\n"))
    {
        token = tokens.substr(0, pos);
        tokens.erase(0, pos + 4);
        LineLabel label(text, sf::Vector2f(), sf::Vector2f(), 0, m_font, LineLabel::Centered);
        if(replacement != "" && token.find("%") != std::string::npos)
            token = utility::replace(token, replacement);
        label.setText(token);
        m_label[m_lines++] = label;
    }
    // single line or last multiline
    LineLabel label(text, sf::Vector2f(), sf::Vector2f(), 0, m_font, LineLabel::Centered);
    if(replacement != "" && tokens.find("%") != std::string::npos)
        tokens = utility::replace(tokens, replacement);
    label.setText(tokens);
    m_label[m_lines++] = label;
}

void ToolTip::update()
{
    if(m_textKey != "")
    {
        auto next = utility::translateKey(m_textKey);
        if(next != m_lastTranslation)
            setText(m_textKey, m_lastReplacement);
    }
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

sf::Vector2f ToolTip::calculateNeededOffset(const sf::Vector2f& position, const sf::RenderWindow& screen)
{
    auto height = m_background[TopLeft].getTextureRect().height;
    auto width = m_background[TopLeft].getTextureRect().width;

    float verticalOffset = 0;
    float horizontalOffset = 0;

    // check left screen border
    if(static_cast<int>(position.x - m_width / 2.f - width + m_offset.x) < 0)
        verticalOffset = static_cast<int>(position.x - m_width / 2.f - width + m_offset.x) * -1.f;

    // check right screen border
    if(static_cast<int>(position.x + m_width / 2.f + m_offset.x) > screen.getSize().x)
    {
        if(verticalOffset == 0)
            verticalOffset = screen.getSize().x - static_cast<int>(position.x - m_width / 2.f - width + m_offset.x);
        else
            throw std::runtime_error(utility::translateKey("ToolTipTooLong"));
    }

    // check upper screen border
    if(static_cast<int>(position.y - height + m_offset.y) < 0)
        horizontalOffset = static_cast<int>(position.y - height + m_offset.y) * -1.f;

    // check bottom screen border
    if(static_cast<int>(position.y + m_height + m_offset.y) > screen.getSize().y)
    {
        if(horizontalOffset == 0)
            horizontalOffset = screen.getSize().y - static_cast<int>(position.y + m_height + m_offset.y);
        else
            throw std::runtime_error(utility::translateKey("ToolTipTooLong"));
    }

    return sf::Vector2f(verticalOffset, horizontalOffset);
}
