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

SubWindow::SubWindow(const int id,
                     const sf::Vector2f& position,
                     const sf::Vector2f& size,
                     const sf::Vector2f& offset,
                     const int innerHeight,
                     const std::vector<std::unique_ptr<MenuElement>>& elements,
                     const SubWindowStyle& style) :
    MenuElement(id, MenuElementType::SubWindow, position, offset),
    m_panel(elements),
    m_style(style),
    m_innerHeight(innerHeight),
    m_size(size),
    m_startValue(0),
    m_endValue(0),
    m_active(false),
    m_zoomFactor(1.f)
{
    auto scrollWidth = static_cast<float>(m_style.scrollbarTop.getTextureRect().width);

    m_windowRect.setSize(sf::Vector2f(size.x - scrollWidth, size.y));
    m_windowRect.setFillColor(sf::Color(100, 100, 100, 70));

    m_sliderRect.setSize(sf::Vector2f(scrollWidth, m_size.y));
    m_sliderRect.setFillColor(sf::Color(40, 40, 40, 70));

    m_positionRect.setPosition(m_size.x - scrollWidth, 0);

    auto positionSize = m_size.y / m_innerHeight;
    if(positionSize > 1.f)
        positionSize = 1.f;

    m_positionRect.setSize(sf::Vector2f(m_sliderRect.getSize().x, floorf(m_size.y * positionSize)));
    m_positionRect.setFillColor(sf::Color(128, 8, 128, 50));

    m_center.x = m_size.x / 2.f;
    m_center.y = m_size.y / 2.f;
}

std::unique_ptr<MenuElement> SubWindow::doClone() const
{
    auto clone = std::unique_ptr<MenuElement>(new SubWindow(getId(), getPosition(), m_size, getOffset(), m_innerHeight, m_panel.getElements(), m_style));
    clone->setVisibleWhenId(getVisibleWhenId());
    return std::move(clone);
}

MenuPanel* SubWindow::getPanel()
{
    return &m_panel;
}

void SubWindow::on(const DrawParameter& params)
{
    m_orginalView = params.getTarget().getView();
    auto currentPosition = getCurrentPosition();
    auto orginalScreenRect = params.getTarget().getSize();
    sf::FloatRect windowViewport;
    windowViewport.width = m_size.x / (orginalScreenRect.x - 0.2f) / m_zoomFactor;
    windowViewport.height = m_size.y / (orginalScreenRect.y - 0.2f) / m_zoomFactor;
    windowViewport.left = currentPosition.x / (orginalScreenRect.x - 0.2f) + windowViewport.width * ((m_zoomFactor - 1.f) / 2.f);
    windowViewport.top = currentPosition.y / (orginalScreenRect.y - 0.2f)  + windowViewport.height * ((m_zoomFactor - 1.f) / 1.6f);
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

void SubWindow::doDraw(const DrawParameter& params)
{
    if(!isVisible())
        return;

    params.getTarget().draw(m_windowRect);
    params.getTarget().draw(m_sliderRect);

    on(params);
    m_panel.draw(params);
    off(params);

    params.getTarget().draw(m_style.scrollbarTop);
    params.getTarget().draw(m_style.scrollbarMiddle);
    params.getTarget().draw(m_style.scrollbarBottom);
}

void SubWindow::updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset)
{
    updateLayout(static_cast<sf::Vector2f>(screen.getSize()));

    auto currentPosition = getCurrentPosition();

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
        if(scroll > m_size.y - m_positionRect.getSize().y)
            scroll = m_size.y - m_positionRect.getSize().y;

        m_positionRect.setPosition(m_positionRect.getPosition().x, scroll);
        m_center.y = floorf(m_size.y / 2.f + sliderPixelToWindowPixel(m_positionRect.getPosition().y));
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
        if(y > m_size.y - m_positionRect.getSize().y)
            y = m_size.y - m_positionRect.getSize().y;
        m_positionRect.setPosition(m_positionRect.getPosition().x, y);

        m_center.y = floorf(m_size.y / 2.f + sliderPixelToWindowPixel(m_positionRect.getPosition().y));
        m_startValue = m_endValue;
    }
    else if(utility::Mouse.leftButtonReleased())
        m_active = false;

    auto pos = m_positionRect.getPosition() + currentPosition;
    m_style.scrollbarTop.setPosition(pos);
    m_style.scrollbarMiddle.setPosition(pos.x, pos.y + m_style.scrollbarTop.getTextureRect().height);
    auto height = m_positionRect.getSize().y
                    - m_style.scrollbarTop.getTextureRect().height
                    - m_style.scrollbarBottom.getTextureRect().height;
    m_style.scrollbarMiddle.setScale(1, height/m_style.scrollbarMiddle.getTextureRect().height);
    m_style.scrollbarBottom.setPosition(pos.x, pos.y + height + m_style.scrollbarTop.getTextureRect().height);

    if(mouseRect.contains(m_cursorPosition))
        m_panel.update(screen, time, getMouseOffset(screen));
    else
        m_panel.update(screen, time, static_cast<sf::Vector2i>(screen.getSize()));
}

float SubWindow::sliderPixelToWindowPixel(float pixel)
{
    return pixel * (m_innerHeight - m_size.y) / (m_sliderRect.getSize().y - m_positionRect.getSize().y);
}

float SubWindow::windowPixelToSliderPixel(float pixel)
{
    return pixel * (m_sliderRect.getSize().y - m_positionRect.getSize().y) / (m_innerHeight - m_size.y);
}

sf::Vector2i SubWindow::getMouseOffset(const sf::RenderWindow& screen)
{
    sf::Vector2i mouseOffset = - static_cast<sf::Vector2i>(getCurrentPosition()) +
                                 sf::Vector2i(static_cast<int>(m_center.x - m_size.x / 2.f), static_cast<int>(m_center.y - m_size.y / 2.f));

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
    auto currentPosition = getCurrentPosition();
    auto rectPosition = m_positionRect.getPosition().y;

    auto scrollWidth = static_cast<float>(m_style.scrollbarTop.getTextureRect().width);

    m_windowRect.setPosition(currentPosition);
    m_sliderRect.setPosition(currentPosition.x + m_size.x - scrollWidth,
                             currentPosition.y);
    m_positionRect.setPosition(m_size.x - scrollWidth,
                               rectPosition);

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

void SubWindow::setLanguage(const std::string& language)
{
    MenuElement::setLanguage(language);
    for(auto it = begin(m_panel.getElements()); it != end(m_panel.getElements()); ++it)
        (*it)->setLanguage(language);
}
