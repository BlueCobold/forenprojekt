#include "InteractiveLabel.hpp"

/*InteractiveLabel::InteractiveLabel() :
    LineLabel("", sf::Vector2f(0,0), sf::Vector2f(0,0), 0, MenuElementType::InteractiveLabel, nullptr)
{}*/

InteractiveLabel::InteractiveLabel(const std::string& text,
                                   const sf::Vector2f& position,
                                   const sf::Vector2f& offset,
                                   const float rotation,
                                   const BitmapFont* font,
                                   const Alignment alignment,
                                   int id) :
    LineLabel(text, position, offset, rotation, MenuElementType::InteractiveLabel, font, alignment, id)
{
}

void InteractiveLabel::setToolTip(const ToolTip& toolTip)
{
    m_toolTip = toolTip;
}

void InteractiveLabel::setToolTipText(const std::string& text)
{
    m_toolTip.setText(text);
}

void InteractiveLabel::update(const sf::RenderWindow& screen, const float time, const sf::Vector2i& mouseOffset)
{
    float offset = 0;
    if(LineLabel::Centered == getAlignment())
        offset = -getWidth() / 2.f;
    else if(LineLabel::Right == getAlignment())
        offset = -getWidth();

    auto position = getPosition() + getOffset() + sf::Vector2f(offset,0);
    auto size = sf::Vector2i(static_cast<int>(getWidth()), getFontSize());
    auto mousePosition = getCursorPosition(screen);
    sf::IntRect hitBox(static_cast<sf::Vector2i>(position), size);
    
    if(hitBox.contains(mousePosition + mouseOffset) && isVisible())
    {
        m_showToolTip = true;
        m_toolTip.setPosition(static_cast<const sf::Vector2f>(mousePosition), screen);
    }
}

void InteractiveLabel::draw(const DrawParameter& params)
{
    LineLabel::draw(params);
}

void InteractiveLabel::drawAdditionalForeground(const DrawParameter& params)
{
    if(m_showToolTip && isVisible())
        m_toolTip.draw(params);

    m_showToolTip = false;
}