#pragma once

#ifndef MENU_ELEMENT_HPP
#define MENU_ELEMENT_HPP

#include "../animation/Cloneable.hpp"
#include "../rendering/Drawable.hpp"
#include "../Input.hpp"
#include "ScreenLocation.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

namespace MenuElementType
{
    enum Type
    {
        Button,
        CheckBox,
        Label,
        Image,
        Slider,
        SubWindow,
        InputBox,
        Animation,
        InteractiveLabel,
        Border
    };
}

/// The basic interface type of all menu elements to unify their common attributes
class MenuElement : public Drawable, public Cloneable<MenuElement>
{
public:

    MenuElement(int id,
                MenuElementType::Type type,
                const sf::Vector2f& position,
                const sf::Vector2f& offset) :
        m_id(id),
        m_type(type),
        m_screenLocation(position, offset),
        m_visible(true),
        m_cursorIsValid(true),
        m_visibleWhen(nullptr),
        m_masterId(-1)
    { }

    MenuElement(int id,
                MenuElementType::Type type,
                ScreenLocation position) :
        m_id(id),
        m_type(type),
        m_screenLocation(position),
        m_visible(true),
        m_cursorIsValid(true),
        m_visibleWhen(nullptr),
        m_masterId(-1)
    { }

    virtual ~MenuElement()
    { }

    void update(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0))
    {
        updated(screen, time, mouseOffset);
    }

    void drawAdditionalForeground(const DrawParameter& params)
    {
        onDrawAdditionalForeground(params);
    }

    void setPosition(const sf::Vector2f& position)
    {
        m_screenLocation.setPosition(position);
        positionSet();
    }

    void setOffset(const sf::Vector2f& offset)
    {
        m_screenLocation.setOffset(offset);
    }

    void setVisible(const bool visible)
    {
        m_visible = visible;
    }

    int getId() const
    {
        return m_id;
    }

    const MenuElementType::Type getType() const
    {
        return m_type;
    }

    const sf::Vector2f& getPosition() const
    {
        return m_screenLocation.getPosition();
    }

    const sf::Vector2f& getOffset() const
    {
        return m_screenLocation.getOffset();
    }

    bool isVisible() const
    {
        return m_visible && (m_visibleWhen == nullptr || m_visibleWhen->isVisible());
    }

    void setVisibleWhenId(const int id)
    {
        m_masterId = id;
    }

    int getVisibleWhenId() const
    {
        return m_masterId;
    }

    void setVisibleWhenSubject(const MenuElement* subject)
    {
        m_visibleWhen = subject;
    }

    void updateLayout(const sf::Vector2f& screenSize)
    {
        m_screenLocation.setScreenSize(screenSize);
        layoutUpdated(screenSize);
    }

    void setCursorIsValid(bool isValid)
    {
        m_cursorIsValid = isValid;
    }

protected:
    bool cursorIsValid() const
    {
        return m_cursorIsValid;
    }

    virtual void positionSet()
    { }

    virtual void layoutUpdated(const sf::Vector2f& screenSize)
    { }

    virtual void updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0))
    { }

    virtual void onDrawAdditionalForeground(const DrawParameter& params)
    { }

    const sf::Vector2f& getCurrentPosition() const
    {
        return m_screenLocation.getCurrentPosition();
    }

    const sf::Vector2i getCursorPosition(const sf::RenderWindow& screen) const
    {
        if(!m_cursorIsValid)
            return sf::Vector2i(INT_MIN, INT_MIN);

#if defined(IOS) || defined(ANDROID)
        return static_cast<sf::Vector2i>(screen.mapPixelToCoords(utility::Mouse.getTouchPosition()));
#else
        return static_cast<sf::Vector2i>(screen.mapPixelToCoords(sf::Mouse::getPosition(screen)));
#endif
    }

private:

    int m_id;
    MenuElementType::Type m_type;
    ScreenLocation m_screenLocation;
    bool m_visible;
    bool m_cursorIsValid;
    const MenuElement* m_visibleWhen;
    int m_masterId;
};

#endif // MENU_ELEMENT_HPP
