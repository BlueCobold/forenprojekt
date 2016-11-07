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
    m_spriteBackground = &m_style.idle.spriteBackground;

    m_sliderPosition.y = position.y + offset.y + m_style.mouseRect.top;

    m_min = m_style.min;
    m_max = m_style.max;
}

std::unique_ptr<MenuElement> Slider::doClone() const
{
    auto clone = std::unique_ptr<Slider>(new Slider(getId(), m_style, getPosition(), getOffset()));
    clone->setVisibleWhenId(getVisibleWhenId());
    return std::move(clone);
}

void Slider::updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset)
{
    updateLayout(static_cast<sf::Vector2f>(screen.getSize()));

    auto currentPosition = getCurrentPosition();
    int x = static_cast<int>(currentPosition.x + m_style.mouseRect.left);
    sf::IntRect sliderRect(x, static_cast<int>(currentPosition.y + m_style.mouseRect.top),
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
        auto mousePos = getCursorPosition(screen);
        if(sliderRect.contains(mousePos + mouseOffset))
        {
            m_active = true;
            m_pick = mousePos.x - sliderRect.left + mouseOffset.x;
        }
    }

    if(!m_active)
    {
        m_spriteSlider = &m_style.idle.spriteSlider;
        m_spriteBackground = &m_style.idle.spriteBackground;
        offset = m_style.idle.sliderOffset;
    }
    else
    {
        auto mousePos = getCursorPosition(screen) + mouseOffset;
        calculateValue(x, mousePos.x - m_pick);
        m_spriteSlider = &m_style.active.spriteSlider;
        m_spriteBackground = &m_style.active.spriteBackground;
        offset = m_style.active.sliderOffset;
    }
    m_sliderPosition.x = (m_value - m_min) * m_style.width / m_max;
    offset.x += x;
    m_spriteSlider->setPosition(m_sliderPosition + offset);
}

void Slider::doDraw(const DrawParameter& params)
{
    if(!isVisible())
        return;

    params.getTarget().draw(*m_spriteBackground);
    params.getTarget().draw(*m_spriteSlider);
}

float Slider::getValue() const
{
    return m_value;
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

void Slider::layoutUpdated(const sf::Vector2f& screenSize)
{
    auto currentPosition = getCurrentPosition();

    float x = currentPosition.x + m_style.mouseRect.left;
    m_sliderPosition.x = (m_value - m_min) * m_style.width / m_max;
    m_sliderPosition.y = currentPosition.y + m_style.mouseRect.top;

    m_style.active.spriteBackground.setPosition(currentPosition + m_style.active.backgroundOffset);
    m_style.idle.spriteBackground.setPosition(currentPosition + m_style.idle.backgroundOffset);
    m_spriteSlider->setPosition(m_sliderPosition + m_style.idle.sliderOffset + sf::Vector2f(x, 0));
}