#include "Slider.hpp"
#include "../Input.hpp"
#include "../Utility.hpp"

Slider::Slider(const int id, const SliderStyle style, const sf::Vector2f& position, const sf::Vector2f& offset) :
    MenuElement(id, MenuElementType::Slider, position, offset),
    m_style(style),
    m_active(false),
    m_pick(0)
{
    m_spriteSlider = &m_style.idle.spriteSlider;
    m_spriteBackround = &m_style.idle.spriteBackround;

    m_sliderPosition.y = position.y + offset.y + m_style.mouseRect.top;

    m_min = m_style.min;
    m_max = m_style.max;

    onPositionChanged();
}

void Slider::update(const sf::RenderWindow& screen)
{
    auto position = getPosition();
    auto eoffset = getOffset();
    int x = static_cast<int>(position.x + eoffset.x + m_style.mouseRect.left);
    sf::IntRect sliderRect(x, static_cast<int>(position.y + eoffset.y + m_style.mouseRect.top),
                           m_style.mouseRect.width,
                           m_style.mouseRect.height);

    sf::Vector2f offset(0, 0);
    if(!utility::Mouse.leftButtonPressed())
    {
        m_active = false;
        m_pick = 0;
    }
    else if(!m_active)
    {
        sliderRect.left += static_cast<int>(m_sliderPosition.x);
        sf::Vector2i mousePos = sf::Mouse::getPosition(screen);
        if(sliderRect.contains(mousePos))
        {
            m_active = true;
            m_pick = mousePos.x - sliderRect.left;
        }
    }

    if(!m_active)
    {
        m_spriteSlider = &m_style.idle.spriteSlider;
        m_spriteBackround = &m_style.idle.spriteBackround;
        offset = m_style.idle.sliderOffset;
    }
    else
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(screen);
        calculateValue(x, mousePos.x - m_pick);
        m_spriteSlider = &m_style.active.spriteSlider;
        m_spriteBackround = &m_style.active.spriteBackround;
        offset = m_style.active.sliderOffset;
    }
    m_sliderPosition.x = (m_value - m_min) * m_style.width / m_max;
    offset.x += x;
    m_spriteSlider->setPosition(m_sliderPosition + offset);
}

void Slider::draw(const DrawParameter& params)
{
    if(!isVisible())
        return;

    params.getTarget().draw(*m_spriteBackround);
    params.getTarget().draw(*m_spriteSlider);
}

float Slider::getValue() const
{
    return m_value;
}

void Slider::onPositionChanged()
{
    auto position = getPosition();
    auto offset = getOffset();
    m_style.active.spriteBackround.setPosition(position + offset + m_style.active.backgroundOffset);
    m_style.idle.spriteBackround.setPosition(position + offset + m_style.idle.backgroundOffset);

    m_sliderPosition.y = position.y + offset.y + m_style.mouseRect.top;
}

void Slider::setValue(const float value)
{
    m_value = value;
}

void Slider::calculateSliderPosition(const sf::IntRect& rect)
{
    float posx = rect.width * (m_value / m_max);
    m_sliderPosition.x = rect.left + posx;
}

void Slider::calculateValue(const int left, const int mousex)
{
    float value = m_min + static_cast<float>(mousex - left) / m_style.width * (m_max - m_min);
    m_value = std::max(std::min(value, m_max), m_min);
}
