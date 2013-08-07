#include "CheckBox.hpp"
#include "../Input.hpp"

CheckBox::CheckBox(int id, CheckBoxStyle style, const sf::Vector2f& position, const sf::Vector2f& offset) :
    MenuElement(id, MenuElementType::CheckBox, position, offset),
    m_style(style),
    m_checked(false)
{
    m_sprite = &m_style.uncheckedStyle.sprite;
    onPositionChanged();
}

void CheckBox::update(const sf::RenderWindow& screen)
{
    auto position = getPosition();
    auto offset = getOffset();
    sf::IntRect checkboxRect(static_cast<int>(position.x + offset.x + m_style.mouseRect.left),
                             static_cast<int>(position.y + offset.y + m_style.mouseRect.top),
                             m_style.mouseRect.width,
                             m_style.mouseRect.height);
    if(utility::Mouse.leftButtonReleased())
    {
        if(checkboxRect.contains(sf::Mouse::getPosition(screen)) && !m_checked)
        {
            m_checked = true;
            m_sprite = &m_style.checkedStyle.sprite;
            onPositionChanged();
        }
        else if(checkboxRect.contains(sf::Mouse::getPosition(screen)) && m_checked)
        {
            m_checked = false;
            m_sprite = &m_style.uncheckedStyle.sprite;
            onPositionChanged();
        }
    }
}

void CheckBox::draw(const DrawParameter& params)
{
    if(!isVisible())
        return;

    params.getTarget().draw(*m_sprite);
}

void CheckBox::onPositionChanged()
{
    auto position = getPosition();
    auto offset = getOffset();
    m_style.uncheckedStyle.sprite.setPosition(position + offset + m_style.uncheckedStyle.spriteOffset);
    m_style.checkedStyle.sprite.setPosition(position + offset + m_style.checkedStyle.spriteOffset);
}

bool CheckBox::getChecked()
{
    return m_checked;
}

void CheckBox::setChecked(bool checked)
{
    m_checked = checked;

    if(m_checked)
    {
        m_sprite = &m_style.checkedStyle.sprite;
        onPositionChanged();
    }
    else
    {
        m_sprite = &m_style.uncheckedStyle.sprite;
        onPositionChanged();
    }
}
