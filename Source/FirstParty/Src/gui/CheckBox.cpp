#include "CheckBox.hpp"
#include "../Input.hpp"

CheckBox::CheckBox(int id, CheckBoxStyle style, const sf::Vector2f& position, const sf::Vector2f& offset) :
    MenuElement(id, MenuElementType::CheckBox, position, offset),
    m_style(style),
    m_checked(false),
    m_hover(false),
    m_showToolTip(false)
{
    m_sprite = &m_style.uncheckedStyle.sprite;
}

std::unique_ptr<MenuElement> CheckBox::doClone() const
{
    auto clone = std::unique_ptr<CheckBox>(new CheckBox(getId(), m_style, getPosition(), getOffset()));
    clone->setVisibleWhenId(getVisibleWhenId());
    clone->m_toolTip = m_toolTip;
    return std::move(clone);
}

void CheckBox::updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset)
{
    updateLayout(static_cast<sf::Vector2f>(screen.getSize()));
    m_toolTip.update();

    auto currentPosition = getCurrentPosition();
    sf::IntRect checkboxRect(static_cast<int>(currentPosition.x + m_style.mouseRect.left),
                             static_cast<int>(currentPosition.y + m_style.mouseRect.top),
                             m_style.mouseRect.width,
                             m_style.mouseRect.height);
    
    auto mousePosition = getCursorPosition(screen);
    if(checkboxRect.contains(getCursorPosition(screen) + mouseOffset))
    {
        m_showToolTip = true;
        m_hover = true;
        m_toolTip.setPosition(static_cast<const sf::Vector2f>(mousePosition), screen);

        if(utility::Mouse.leftButtonReleased())
            m_checked = !m_checked;
    }
    else
    {
        m_showToolTip = false;
        m_hover = false;
    }

    if(m_hover && !m_checked)
        m_sprite = &m_style.hoverStyle.sprite;
    else if(m_checked)
        m_sprite = &m_style.checkedStyle.sprite;
    else
        m_sprite = &m_style.uncheckedStyle.sprite;
}

void CheckBox::doDraw(const DrawParameter& params)
{
    if(!isVisible())
        return;

    params.getTarget().draw(*m_sprite);
}

void CheckBox::onDrawAdditionalForeground(const DrawParameter& params)
{
    if(m_showToolTip && isVisible())
        m_toolTip.draw(params);
}

bool CheckBox::getChecked()
{
    return m_checked;
}

void CheckBox::setChecked(bool checked)
{
    m_checked = checked;

    if(m_hover && !m_checked)
        m_sprite = &m_style.hoverStyle.sprite;
    else if(m_checked)
    {
        m_sprite = &m_style.checkedStyle.sprite;
        m_hover = false;
    }
    else
        m_sprite = &m_style.uncheckedStyle.sprite;

}

void CheckBox::setToolTip(const ToolTip& toolTip)
{
    m_toolTip = toolTip;
}

void CheckBox::setToolTipText(const std::string& text, const std::string& replacement)
{
    m_toolTip.setText(text, replacement);
}

void CheckBox::layoutUpdated(const sf::Vector2f& screenSize)
{
    auto currentPosition = getCurrentPosition();
    m_style.uncheckedStyle.sprite.setPosition(currentPosition + m_style.uncheckedStyle.spriteOffset);
    m_style.checkedStyle.sprite.setPosition(currentPosition + m_style.checkedStyle.spriteOffset);
    m_style.hoverStyle.sprite.setPosition(currentPosition + m_style.hoverStyle.spriteOffset);
}
