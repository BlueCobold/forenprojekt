#include "CheckBox.hpp"
#include "../Input.hpp"

CheckBox::CheckBox(int id,
                   CheckBoxStyle style,
                   const ScreenLocation& position,
                   const ScreenScale& scale) :
    MenuElement(id, MenuElementType::CheckBox, position),
    m_style(style),
    m_checked(false),
    m_hover(false),
    m_showToolTip(false),
    m_scale(scale)
{
    m_sprite = &m_style.uncheckedStyle.sprite;
}

std::unique_ptr<MenuElement> CheckBox::onClone() const
{
    auto clone = std::unique_ptr<CheckBox>(new CheckBox(getId(), m_style, getPosition(), m_scale));
    clone->m_toolTip = m_toolTip;
    return std::move(clone);
}

void CheckBox::registerOnChange(std::function<void (const CheckBox& sender)> callback)
{
    m_callback = callback;
}

void CheckBox::updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset)
{
    updateLayout(sf::Vector2f(screen.getSize()));
    m_toolTip.update();

    auto position = getCurrentPosition();
    auto &scale = m_scale.getCurrentScale();
    sf::FloatRect checkboxRect(position.x + scale.x * m_style.mouseRect.left,
                               position.y + scale.y * m_style.mouseRect.top,
                               scale.x * m_style.mouseRect.width,
                               scale.y * m_style.mouseRect.height);

    auto mousePosition = getCursorPosition(screen);
    if(cursorIsValid() && checkboxRect.contains(sf::Vector2f(getCursorPosition(screen) + mouseOffset)))
    {
        m_showToolTip = true;
        m_hover = true;
        m_toolTip.setPosition(sf::Vector2f(mousePosition), screen);

        if(utility::Mouse.leftButtonReleased())
        {
            m_checked = !m_checked;
            if(m_callback)
                m_callback(*this);
        }
    }
    else
    {
        m_showToolTip = false;
        m_hover = false;
    }

    if(m_hover && !m_checked)
        m_sprite = &m_style.hoverStyle.sprite;
    else if(m_hover)
        m_sprite = &m_style.checkedHoverStyle.sprite;
    else if(m_checked)
        m_sprite = &m_style.checkedStyle.sprite;
    else
        m_sprite = &m_style.uncheckedStyle.sprite;
}

void CheckBox::onDrawElement(const DrawParameter& params)
{
    m_sprite->draw(params);
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

sf::Vector2f operator*(const sf::Vector2f& l, const sf::Vector2f& r)
{
    return sf::Vector2f(l.x*r.x, l.y*r.y);
}

void CheckBox::layoutUpdated(const sf::Vector2f& screenSize)
{
    auto pos = getCurrentPosition();
    m_scale.setScreenSize(screenSize);
    auto currentPosition = sf::Vector2f(floorf(pos.x), floorf(pos.y));

    m_style.uncheckedStyle.sprite.setPosition(currentPosition
                                            + m_scale.getCurrentScale() * m_style.uncheckedStyle.spriteOffset);
    m_style.uncheckedStyle.sprite.setScale(m_scale.getCurrentScale());

    m_style.checkedStyle.sprite.setPosition(currentPosition
                                          + m_scale.getCurrentScale() * m_style.checkedStyle.spriteOffset);
    m_style.checkedStyle.sprite.setScale(m_scale.getCurrentScale());

    m_style.hoverStyle.sprite.setPosition(currentPosition
                                        + m_scale.getCurrentScale() * m_style.hoverStyle.spriteOffset);
    m_style.hoverStyle.sprite.setScale(m_scale.getCurrentScale());

    m_style.checkedHoverStyle.sprite.setPosition(currentPosition
                                               + m_scale.getCurrentScale() * m_style.checkedHoverStyle.spriteOffset);
    m_style.checkedHoverStyle.sprite.setScale(m_scale.getCurrentScale());
}
