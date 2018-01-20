#include "InteractiveLabel.hpp"

InteractiveLabel::InteractiveLabel(const std::string& text,
                                   const ScreenLocation& position,
                                   const float rotation,
                                   const BitmapFont* font,
                                   const Alignment alignment,
                                   int id) :
    LineLabel(text, position, ScreenScale(), rotation, MenuElementType::InteractiveLabel, font, alignment, id)
{
}

std::unique_ptr<MenuElement> InteractiveLabel::onClone() const
{
    auto clone = std::unique_ptr<InteractiveLabel>(new InteractiveLabel(
                                                            getText(),
                                                            getPosition(),
                                                            getRotation(),
                                                            getFont(),
                                                            getAlignment(),
                                                            getId()));
    clone->m_toolTip = m_toolTip;
    return std::move(clone);
}

void InteractiveLabel::setToolTip(const ToolTip& toolTip)
{
    m_toolTip = toolTip;
}

void InteractiveLabel::setToolTipText(const std::string& text, const std::string& replacement)
{
    m_toolTip.setText(text, replacement);
}

void InteractiveLabel::updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset)
{
    LineLabel::updated(screen, time, mouseOffset);

    float offset = 0;
    if(LineLabel::Centered == getAlignment())
        offset = -getWidth() / 2.f;
    else if(LineLabel::Right == getAlignment())
        offset = -getWidth();

    auto position = getCurrentPosition() + sf::Vector2f(offset,0);
    sf::Vector2f size(getWidth(), static_cast<float>(getFontSize()));
    auto mousePosition = getCursorPosition(screen);
    sf::FloatRect hitBox(position, size);
    
    m_showToolTip = cursorIsValid() && hitBox.contains(sf::Vector2f(mousePosition + mouseOffset)) && isVisible();
    if(m_showToolTip)
    {
        m_toolTip.setPosition(sf::Vector2f(mousePosition), screen);
        m_toolTip.update();
    }
}

void InteractiveLabel::onDrawAdditionalForeground(const DrawParameter& params)
{
    LineLabel::onDrawAdditionalForeground(params);

    if(m_showToolTip && isVisible())
        m_toolTip.draw(params);
}
