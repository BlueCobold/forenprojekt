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

std::unique_ptr<MenuElement> InteractiveLabel::doClone() const
{
    auto clone = std::unique_ptr<InteractiveLabel>(new InteractiveLabel(
                                                            getText(),
                                                            getPosition(),
                                                            getOffset(),
                                                            getRotation(),
                                                            getFont(),
                                                            getAlignment(),
                                                            getId()));
    clone->setVisibleWhenId(getVisibleWhenId());
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
    auto size = sf::Vector2i(static_cast<int>(getWidth()), getFontSize());
    auto mousePosition = getCursorPosition(screen);
    sf::IntRect hitBox(static_cast<sf::Vector2i>(position), size);
    
    m_showToolTip = cursorIsValid() && hitBox.contains(mousePosition + mouseOffset) && isVisible();
    if(m_showToolTip)
    {
        m_toolTip.setPosition(static_cast<const sf::Vector2f>(mousePosition), screen);
        m_toolTip.update();
    }
}

void InteractiveLabel::onDrawAdditionalForeground(const DrawParameter& params)
{
    LineLabel::onDrawAdditionalForeground(params);

    if(m_showToolTip && isVisible())
        m_toolTip.draw(params);
}
