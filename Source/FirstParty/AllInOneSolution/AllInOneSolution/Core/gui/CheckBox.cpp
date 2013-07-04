#include "CheckBox.hpp"
#include "../Input.hpp"

CheckBox::CheckBox(int id, CheckBoxStyle style, const sf::Vector2f& position, const sf::Vector2f& offset) :
    m_id(id),
    m_position(position),
    m_offset(offset),
    m_style(style),
    m_checked(false)
{
    m_sprite = &m_style.uncheckedStyle.sprite;

    setPosition(position);
}
void CheckBox::update(const sf::RenderWindow& screen)
{
    sf::IntRect checkboxRect(static_cast<int>(m_position.x + m_offset.x + m_style.mouseRect.left),
                             static_cast<int>(m_position.y + m_offset.y + m_style.mouseRect.top),
                             m_style.mouseRect.width,
                             m_style.mouseRect.height);
    if(utility::Mouse.leftButtonReleased())
    {
        if(checkboxRect.contains(sf::Mouse::getPosition(screen)) && !m_checked)
        {
            m_checked = true;
            m_sprite = &m_style.checkedStyle.sprite;
            setPosition(m_position);
        }
        else if(checkboxRect.contains(sf::Mouse::getPosition(screen)) && m_checked)
        {
            m_checked = false;
            m_sprite = &m_style.uncheckedStyle.sprite;
            setPosition(m_position);
        }
    }
}

void CheckBox::draw(const DrawParameter& params)
{
    params.getTarget().draw(*m_sprite);
}

int CheckBox::getId() const
{
    return m_id;
}

void CheckBox::setPosition(sf::Vector2f position)
{
    m_position = position;

    m_style.uncheckedStyle.sprite.setPosition(m_position + m_offset + m_style.uncheckedStyle.spriteOffset);

    m_style.checkedStyle.sprite.setPosition(m_position + m_offset + m_style.checkedStyle.spriteOffset);
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
        setPosition(m_position);
    }
    else
    {
        m_sprite = &m_style.uncheckedStyle.sprite;
        setPosition(m_position);
    }
}
