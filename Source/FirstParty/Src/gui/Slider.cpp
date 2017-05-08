#include "Slider.hpp"
#include "../Input.hpp"
#include "../Utility.hpp"

Slider::Slider(const int id, const SliderStyle style, const ScreenLocation& position) :
    MenuElement(id, MenuElementType::Slider, position),
    m_style(style),
    m_active(false),
    m_pick(0)
{
    m_button = &m_style.idle.button;
    m_background = &m_style.idle.background;

    m_buttonPosition.y = position.getCurrentPosition().y + m_style.mouseRect.top;

    m_min = m_style.min;
    m_max = m_style.max;
}

std::unique_ptr<MenuElement> Slider::onClone() const
{
    return std::unique_ptr<Slider>(new Slider(getId(), m_style, getPosition()));
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
    if(!utility::Mouse.leftButtonPressed() || !cursorIsValid())
    {
        m_active = false;
        m_pick = 0;
    }
    else if(!m_active)
    {
        sliderRect.left += static_cast<int>(m_buttonPosition.x);
        auto mousePos = getCursorPosition(screen);
        if(cursorIsValid() && sliderRect.contains(mousePos + mouseOffset))
        {
            m_active = true;
            m_pick = mousePos.x - sliderRect.left + mouseOffset.x;
        }
    }

    if(!m_active)
    {
        m_button = &m_style.idle.button;
        m_background = &m_style.idle.background;
        offset = m_style.idle.buttonOffset;
    }
    else
    {
        auto mousePos = getCursorPosition(screen) + mouseOffset;
        calculateValue(x, mousePos.x - m_pick);
        m_button = &m_style.active.button;
        m_background = &m_style.active.background;
        offset = m_style.active.buttonOffset;
    }
    m_buttonPosition.x = (m_value - m_min) * m_style.width / m_max;
    offset.x += x;
    m_button->setPosition(m_buttonPosition + offset);
}

void Slider::onDrawElement(const DrawParameter& params)
{
    m_background->draw(params);
    m_button->draw(params);
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
    m_buttonPosition.x = rect.left + posx;
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
    m_buttonPosition.x = (m_value - m_min) * m_style.width / m_max;
    m_buttonPosition.y = currentPosition.y + m_style.mouseRect.top;

    m_style.active.background.setPosition(currentPosition + m_style.active.backgroundOffset);
    m_style.idle.background.setPosition(currentPosition + m_style.idle.backgroundOffset);
    m_button->setPosition(m_buttonPosition + m_style.idle.buttonOffset + sf::Vector2f(x, 0));
}
