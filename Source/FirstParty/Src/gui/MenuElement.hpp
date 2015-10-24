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
        InteractiveLabel
    };
}

/// The basic interface type of all menu elements to unify their common attributes
class MenuElement : public Drawable, public Cloneable<MenuElement>
{
public:

    MenuElement(const int id,
                const MenuElementType::Type type,
                const sf::Vector2f& position,
                const sf::Vector2f& offset) :
        m_id(id),
        m_type(type),
        m_screenLocation(position, offset),
        m_visible(true),
        m_visibleWhen(nullptr),
        m_masterId(-1)
    { }

    virtual ~MenuElement()
    { }

    virtual void update(const sf::RenderWindow& screen, const float time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0))
    { }

    virtual void draw(const DrawParameter& params) override = 0;
    virtual void drawAdditionalForeground(const DrawParameter& params)
    { }

    void setPosition(const sf::Vector2f& position)
    {
        m_screenLocation.setPosition(position);
    }

    void setVisible(const bool visible)
    {
        m_visible = visible;
    }

    virtual void setPosition(const float x, const float y)
    {
        m_screenLocation.setPosition(sf::Vector2f(x, y));
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

    virtual void setOffset(const sf::Vector2f& offset)
    {
        m_screenLocation.setOffset(offset);
    }
protected:
    virtual void updateLayout(const sf::Vector2f& screenSize)
    {
        m_screenLocation.setScreenSize(screenSize);
    }

    const sf::Vector2f& getOffset() const
    {
        return m_screenLocation.getOffset();
    }

    const sf::Vector2f& getCurrentPosition() const
    {
        return m_screenLocation.getCurrentPosition();
    }

    const sf::Vector2i getCursorPosition(const sf::RenderWindow& screen) const
    {
#ifdef IOS
        return utility::Mouse.getTouchPosition();
#else
        return sf::Mouse::getPosition(screen);
#endif
    }

private:

    int m_id;
    MenuElementType::Type m_type;
    ScreenLocation m_screenLocation;
    bool m_visible;
    const MenuElement* m_visibleWhen;
    int m_masterId;
};

#endif // MENU_ELEMENT_HPP
