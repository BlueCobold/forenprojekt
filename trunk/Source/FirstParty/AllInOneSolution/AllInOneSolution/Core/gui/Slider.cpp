#include "Slider.hpp"
#include "../Input.hpp"
#include "../Utility.hpp"

Slider::Slider(int id, SliderStyle style, const sf::Vector2f& position, const sf::Vector2f& offset) :
    m_id(id),
    m_position(position),
    m_offset(offset),
    m_style(style)
{
    m_label = &m_style.idle.label;

    m_spriteSlider = &m_style.idle.spriteSlider;
    m_spriteBackround = &m_style.idle.spriteBackround;

    m_sliderPosition.y = m_position.y + m_offset.y + m_style.mouseRect.top;

    m_min = m_style.min;
    m_max = m_style.max;
}

void Slider::update(const sf::RenderWindow& screen)
{
    sf::IntRect sliderRect(static_cast<int>(m_position.x + m_offset.x + m_style.mouseRect.left),
                           static_cast<int>(m_position.y + m_offset.y + m_style.mouseRect.top),
                           m_style.mouseRect.width,
                           m_style.mouseRect.height);

    if(utility::Mouse.leftButtonPressed() && sliderRect.contains(sf::Mouse::getPosition(screen)))
    {
        calculateValue(sliderRect, sf::Mouse::getPosition(screen));
        m_label = &m_style.active.label;
        m_spriteSlider = &m_style.active.spriteSlider;
        m_spriteBackround = &m_style.active.spriteBackround;
    }
    else
    {
        m_label = &m_style.idle.label;
        m_spriteSlider = &m_style.idle.spriteSlider;
        m_spriteBackround = &m_style.idle.spriteBackround;
    }
    calculateSliderPosition(sliderRect);
    m_spriteSlider->setPosition(m_sliderPosition);
}

void Slider::draw(const DrawParameter& params)
{
    params.getTarget().draw(*m_spriteBackround);
    params.getTarget().draw(*m_spriteSlider);
    m_label->draw(params);
}

int Slider::getId() const
{
    return m_id;
}

float Slider::getValue() const
{
    return m_value;
}
void Slider::setPosition(sf::Vector2f position)
{
    m_position = position;

    m_style.active.spriteBackround.setPosition(m_position + m_offset + m_style.active.spriteOffset);
    m_style.active.label.setPosition(m_position + m_offset + m_style.active.textOffset);

    m_style.idle.spriteBackround.setPosition(m_position + m_offset + m_style.idle.spriteOffset);
    m_style.idle.label.setPosition(m_position + m_offset + m_style.idle.textOffset);

    m_sliderPosition.y = m_position.y + m_offset.y + m_style.mouseRect.top - 
                         m_style.idle.spriteSlider.getTextureRect().height / 2.f +
                         m_style.idle.spriteBackround.getTextureRect().height / 2.f;
}

void Slider::setValue(float value)
{
    m_value = value;
}

void Slider::calculateSliderPosition(sf::IntRect rect)
{
    float posx = rect.width * (m_value / m_max);
    m_sliderPosition.x = rect.left + posx;
}

void Slider::calculateValue(sf::IntRect rect, sf::Vector2i mousePosition)
{
    float value = static_cast<float>(mousePosition.x - rect.left) / rect.width * m_max;
    m_value = value;
}