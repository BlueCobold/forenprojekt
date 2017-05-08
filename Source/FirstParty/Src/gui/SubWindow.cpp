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

#include <algorithm>

SubWindow::SubWindow(int id,
                     const ScreenLocation& position,
                     const ScreenSize& screenSize,
                     int innerHeight,
                     const std::vector<std::unique_ptr<MenuElement>>& elements,
                     const SubWindowStyle& style) :
    MenuElement(id, MenuElementType::SubWindow, position),
    m_panel(elements),
    m_style(style),
    m_innerHeight(innerHeight),
    m_size(screenSize),
    m_startValue(0),
    m_endValue(0),
    m_active(false),
    m_zoomFactor(1.f)
{
    auto scrollWidth = static_cast<float>(m_style.scrollbarTop.getTextureRect().width);
    auto size = screenSize.getCurrentSize();

    m_windowRect.setSize(sf::Vector2f(size.x - scrollWidth, size.y));
    m_windowRect.setFillColor(sf::Color(100, 100, 100, 70));

    m_sliderRect.setSize(sf::Vector2f(scrollWidth, m_size.getCurrentSize().y));
    m_sliderRect.setFillColor(sf::Color(40, 40, 40, 70));

    m_positionRect.setPosition(size.x - scrollWidth, 0);

    auto positionSize = size.y / m_innerHeight;
    if(positionSize > 1.f)
        positionSize = 1.f;

    m_positionRect.setSize(sf::Vector2f(m_sliderRect.getSize().x, floorf(size.y * positionSize)));
    m_positionRect.setFillColor(sf::Color(128, 8, 128, 50));

    m_center.x = size.x / 2.f;
    m_center.y = size.y / 2.f;
}

std::unique_ptr<MenuElement> SubWindow::onClone() const
{
    return std::unique_ptr<MenuElement>(new SubWindow(getId(), getPosition(), m_size, m_innerHeight, m_panel.getElements(), m_style));
}

MenuPanel* SubWindow::getPanel()
{
    return &m_panel;
}

void SubWindow::on(const DrawParameter& params)
{
    m_orginalView = params.getTarget().getView();
    auto currentPosition = getCurrentPosition() - m_size.getCurrentSize() / 2.f;
    auto size = m_size.getCurrentSize();
    auto orginalScreenRect = params.getTarget().getSize();
    sf::FloatRect windowViewport;
    windowViewport.width = std::min(size.x / (orginalScreenRect.x - 0.0002f) / m_zoomFactor, 1.0f);
    windowViewport.height = std::min(size.y / (orginalScreenRect.y - 0.0002f) / m_zoomFactor, 1.0f);
    windowViewport.left = currentPosition.x / (orginalScreenRect.x - 0.0002f) + windowViewport.width * ((m_zoomFactor - 1.f) / 2.f);
    windowViewport.top = currentPosition.y / (orginalScreenRect.y - 0.0002f)  + windowViewport.height * ((m_zoomFactor - 1.f) / 1.6f);
    sf::View windowView;
    windowView.setViewport(windowViewport);
    windowView.setCenter(m_center);
    windowView.setSize(size);
    params.getTarget().setView(windowView);
}

void SubWindow::off(const DrawParameter& params)
{
    params.getTarget().setView(m_orginalView);
}

void SubWindow::onDrawElement(const DrawParameter& params)
{
    params.getTarget().draw(m_windowRect);
    params.getTarget().draw(m_sliderRect);

    on(params);
    m_panel.draw(params);
    off(params);

    m_style.scrollbarTop.draw(params);
    m_style.scrollbarMiddle.draw(params);
    m_style.scrollbarBottom.draw(params);
}

void SubWindow::updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset)
{
    updateLayout(static_cast<sf::Vector2f>(screen.getSize()));

    auto currentPosition = getCurrentPosition() - m_size.getCurrentSize() / 2.f;

    sf::IntRect mouseRect(static_cast<sf::Vector2i>(m_windowRect.getPosition()) + mouseOffset, static_cast<sf::Vector2i>(m_windowRect.getSize()));
    sf::IntRect sliderRect(static_cast<sf::Vector2i>(m_positionRect.getPosition() + currentPosition), static_cast<sf::Vector2i>(m_positionRect.getSize()));

    auto moveDistance = m_cursorPosition - getCursorPosition(screen);
    m_cursorPosition = getCursorPosition(screen);

    if(mouseRect.contains(m_cursorPosition))
    {
        auto scroll = m_positionRect.getPosition().y;

        if(utility::Mouse.isWheelMovedDown())
            scroll += ceilf(windowPixelToSliderPixel(15));
        else if(utility::Mouse.isWheelMovedUp())
            scroll += ceilf(windowPixelToSliderPixel(-15));
        else if(utility::Mouse.leftButtonPressed())
            scroll += windowPixelToSliderPixel(moveDistance.y * 2.f);

        if(scroll < 0)
            scroll = 0;
        if(scroll > m_size.getCurrentSize().y - m_positionRect.getSize().y)
            scroll = m_size.getCurrentSize().y - m_positionRect.getSize().y;

        m_positionRect.setPosition(m_positionRect.getPosition().x, scroll);
    }

    if(sliderRect.contains(m_cursorPosition) && utility::Mouse.leftButtonDown())
    {
        m_startValue = m_cursorPosition.y;
        m_active = true;
    }
    else if(utility::Mouse.leftButtonPressed() && m_active)
    {
        m_endValue = m_cursorPosition.y;
        auto y = m_positionRect.getPosition().y + m_endValue - m_startValue;
        if(y < 0)
            y = 0;
        if(y > m_size.getCurrentSize().y - m_positionRect.getSize().y)
            y = m_size.getCurrentSize().y - m_positionRect.getSize().y;
        m_positionRect.setPosition(m_positionRect.getPosition().x, y);

        m_startValue = m_endValue;
    }
    else if(utility::Mouse.leftButtonReleased())
        m_active = false;
    
    m_center.y = m_size.getCurrentSize().y / 2.f + sliderPixelToWindowPixel(m_positionRect.getPosition().y);

    auto pos = m_positionRect.getPosition() + currentPosition;
    m_style.scrollbarTop.setPosition(pos);
    m_style.scrollbarMiddle.setPosition(pos.x, pos.y + m_style.scrollbarTop.getTextureRect().height);
    auto height = m_positionRect.getSize().y
                    - m_style.scrollbarTop.getTextureRect().height
                    - m_style.scrollbarBottom.getTextureRect().height;
    m_style.scrollbarMiddle.setScale(1, height/m_style.scrollbarMiddle.getTextureRect().height);
    m_style.scrollbarBottom.setPosition(pos.x, pos.y + height + m_style.scrollbarTop.getTextureRect().height);

    m_panel.setCursorIsValid(mouseRect.contains(m_cursorPosition));
    m_panel.update(screen, time, getMouseOffset(screen));
}

float SubWindow::sliderPixelToWindowPixel(float pixel)
{
    auto delta = m_sliderRect.getSize().y - m_positionRect.getSize().y;
    if(delta > 0)
        return pixel * (m_innerHeight - m_size.getCurrentSize().y) / delta;
    return pixel;
}

float SubWindow::windowPixelToSliderPixel(float pixel)
{
    return pixel * (m_sliderRect.getSize().y - m_positionRect.getSize().y) / (m_innerHeight - m_size.getCurrentSize().y);
}

sf::Vector2i SubWindow::getMouseOffset(const sf::RenderWindow& screen)
{
    sf::Vector2i mouseOffset = - static_cast<sf::Vector2i>(getCurrentPosition() - m_size.getCurrentSize() / 2.f) +
                                 sf::Vector2i(static_cast<int>(m_center.x - m_size.getCurrentSize().x / 2.f),
                                              static_cast<int>(m_center.y - m_size.getCurrentSize().y / 2.f));

    return mouseOffset;
}

void SubWindow::onDrawAdditionalForeground(const DrawParameter& params)
{
    m_panel.drawAdditionalForeground(params);
}

int SubWindow::getInnerHeight() const
{
    return m_innerHeight;
}

void SubWindow::setInnerHeight(int innerHeight)
{
    m_innerHeight = innerHeight;
}

void SubWindow::layoutUpdated(const sf::Vector2f& screenSize)
{
    m_size.setScreenSize(screenSize);
    auto size = m_size.getCurrentSize();
    m_center.x = size.x / 2.f;
    m_center.y = size.y / 2.f;

    auto scrollWidth = static_cast<float>(m_style.scrollbarTop.getTextureRect().width);
    
    auto currentPosition = getCurrentPosition() - m_size.getCurrentSize() / 2.f;
    auto rectPosition = m_positionRect.getPosition().y;

    m_windowRect.setSize(sf::Vector2f(size.x - scrollWidth, size.y));
    m_windowRect.setPosition(currentPosition);

    m_sliderRect.setSize(sf::Vector2f(scrollWidth, size.y));
    m_sliderRect.setPosition(currentPosition.x + m_size.getCurrentSize().x - scrollWidth,
                             currentPosition.y);

    auto positionSize = size.y / m_innerHeight;
    if(positionSize > 1.f)
        positionSize = 1.f;
    m_positionRect.setSize(sf::Vector2f(m_sliderRect.getSize().x, floorf(size.y * positionSize)));
    m_positionRect.setPosition(size.x - scrollWidth, rectPosition);

    auto position = m_positionRect.getPosition() + currentPosition;
    m_style.scrollbarTop.setPosition(position);
    m_style.scrollbarMiddle.setPosition(position.x, position.y + m_style.scrollbarTop.getTextureRect().height);
    auto height = m_positionRect.getSize().y
                    - m_style.scrollbarTop.getTextureRect().height
                    - m_style.scrollbarBottom.getTextureRect().height;
    m_style.scrollbarMiddle.setScale(1, height/m_style.scrollbarMiddle.getTextureRect().height);
    m_style.scrollbarBottom.setPosition(position.x, position.y + height + m_style.scrollbarTop.getTextureRect().height);
}

void SubWindow::setZoomFactor(float zoomFactor)
{
    m_zoomFactor = zoomFactor;
}
