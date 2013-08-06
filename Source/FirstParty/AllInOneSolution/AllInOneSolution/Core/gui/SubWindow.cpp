#include "SubWindow.hpp"

#include "Button.hpp"
#include "CheckBox.hpp"
#include "LineLabel.hpp"
#include "MenuSprite.hpp"
#include "Slider.hpp"

#include "../gui/Menu.hpp"
#include "../Input.hpp"
#include "../rendering/DrawParameter.hpp"
#include "../Utility.hpp"

SubWindow::SubWindow(const sf::Vector2f& position,
                     const sf::Vector2f& size,
                     const sf::Vector2f& innerPosition,
                     const sf::Vector2f& offset,
                     const int innerHeight,
                     const MenuElements& elements) :
    MenuElement(-1, MenuElementType::SubWindow, position, offset),
    m_panel(elements, position),
    m_size(size),
    m_innerHeight(innerHeight),
    m_innerPosition(innerPosition),
    m_center(sf::Vector2f(0, 0)),
    m_startValue(0),
    m_endValue(0),
    m_active(false)
{
    m_windowRect.setPosition(position.x + offset.x,
                             position.y + offset.y);
    m_windowRect.setSize(size);
    m_windowRect.setFillColor(sf::Color(255,128,128,80));

    m_sliderRect.setPosition(position.x + offset.x + m_size.x,
                             position.y + offset.y);
    m_sliderRect.setSize(sf::Vector2f(14, m_size.y));
    m_sliderRect.setFillColor(sf::Color(128,255,128,128));

    m_positionRect.setPosition(position.x + offset.x + m_size.x + 2,
                               position.y + offset.y);

    float positionSize = 1.f - (m_innerHeight - m_size.y) / m_innerHeight;
    if(positionSize > 1.f)
        positionSize = 1.f;

    m_positionRect.setSize(sf::Vector2f(10, m_size.y * positionSize));
    m_positionRect.setFillColor(sf::Color(128,128,255,128));

    m_center.x = m_innerPosition.x + m_size.x / 2.f;
    m_center.y = m_innerPosition.y + m_size.y / 2.f;
}

void SubWindow::on(const DrawParameter& params)
{
    m_orginalView = params.getTarget().getView();
    auto position = getPosition();
    auto offset = getOffset();
    auto orginalScreenRect = params.getTarget().getSize();
    sf::FloatRect windowViewport;
    windowViewport.left = (position.x + offset.x) / (orginalScreenRect.x - 0.2f);
    windowViewport.top = (position.y + offset.y) / (orginalScreenRect.y - 0.2f);
    windowViewport.width = m_size.x / (orginalScreenRect.x - 0.2f);
    windowViewport.height = m_size.y / (orginalScreenRect.y - 0.2f);
    sf::View windowView;
    windowView.setViewport(windowViewport);
    windowView.setCenter(m_center);
    windowView.setSize(m_size);
    params.getTarget().setView(windowView);
}

void SubWindow::off(const DrawParameter& params)
{
    params.getTarget().setView(m_orginalView);
}

void SubWindow::draw(const DrawParameter& params)
{
    if(!isVisible())
        return;

    params.getTarget().draw(m_windowRect);
    params.getTarget().draw(m_sliderRect);
    params.getTarget().draw(m_positionRect);

    on(params);
    m_panel.draw(params);
    off(params);    
}

void SubWindow::update(const sf::RenderWindow& screen)
{
    sf::IntRect mouseRect(static_cast<sf::Vector2i>(m_windowRect.getPosition()), static_cast<sf::Vector2i>(m_windowRect.getSize()));
    sf::IntRect sliderRect(static_cast<sf::Vector2i>(m_positionRect.getPosition()), static_cast<sf::Vector2i>(m_positionRect.getSize()));

    if(mouseRect.contains(sf::Mouse::getPosition(screen)) && utility::Mouse.isWheelMovedDown())
    {
        if((m_center.y + percentToWindowPixels()) < (m_innerPosition.y + m_innerHeight - m_size.y / 2.f))
            m_center.y += percentToWindowPixels();
        else
            m_center.y = m_innerPosition.y + m_innerHeight - m_size.y / 2.f;
    }
    else if(mouseRect.contains(sf::Mouse::getPosition(screen)) && utility::Mouse.isWheelMovedUp())
    {
        if((m_center.y - percentToWindowPixels()) > (m_innerPosition.y + m_size.y / 2.f))
            m_center.y -= percentToWindowPixels();
        else
            m_center.y = m_innerPosition.y + m_size.y / 2.f;
    }

    if(sliderRect.contains(sf::Mouse::getPosition(screen)) && utility::Mouse.leftButtonDown())
    {
        m_startValue = sf::Mouse::getPosition().y;
        m_active = true;
    }
    else if(utility::Mouse.leftButtonPressed() && m_active)
    {
        m_endValue = sf::Mouse::getPosition().y - m_startValue;
        m_center.y += sliderPixelToWindowPixel(m_endValue);
        if(m_center.y < (m_innerPosition.y + m_size.y / 2.f))
            m_center.y = m_innerPosition.y + m_size.y / 2.f;
        else if(m_center.y > (m_innerPosition.y + m_innerHeight - m_size.y / 2.f))
            m_center.y = m_innerPosition.y + m_innerHeight - m_size.y / 2.f;
    }
    else if(utility::Mouse.leftButtonReleased())
        m_active = false;

    setSliderPosition();
}

void SubWindow::onPositionChanged()
{
    auto position = getPosition();
    auto offset = getOffset();
    m_windowRect.setPosition(position.x + offset.x,
                             position.y + offset.y);
    m_sliderRect.setPosition(position.x + offset.x + m_size.x,
                             position.y + offset.y);
    m_positionRect.setPosition(position.x + offset.x + m_size.x + 2,
                               position.y + offset.y);
}

void SubWindow::setInnerPosition(const sf::Vector2f& position)
{
    m_innerPosition = position;
    m_panel.setPosition(position);
}

float SubWindow::percentToWindowPixels(float percent)
{
    return static_cast<float>(m_innerHeight - m_size.y) / 100.f * percent;
}

float SubWindow::sliderPixelToWindowPixel(int pixel)
{
    float pixelPercent = static_cast<float>(pixel / static_cast<float>(m_sliderRect.getSize().y - m_positionRect.getSize().y));
    return percentToWindowPixels(pixelPercent);
}

void SubWindow::setSliderPosition()
{
    auto position = getPosition();
    auto offset = getOffset();
    float currentPercent = (m_innerPosition.y + m_size.y / 2.f - m_center.y) / (m_innerHeight - m_size.y) ;
    int currentPosition = static_cast<int>((m_sliderRect.getSize().y - m_positionRect.getSize().y) * -currentPercent);
    m_positionRect.setPosition(position.x + offset.x + m_size.x + 2,
                                position.y + offset.y + currentPosition);
}
