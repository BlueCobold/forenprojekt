#pragma once

#ifndef MENU_ELEMENT_HPP
#define MENU_ELEMENT_HPP

#include "../rendering/Drawable.hpp"

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
        SubWindow
    };
}

/// The basic interface type of all menu elements to unify their common attributes
class MenuElement : public Drawable
{
public:

    MenuElement(const int id,
                const MenuElementType::Type type,
                const sf::Vector2f& position,
                const sf::Vector2f& offset) :
        m_id(id),
        m_type(type),
        m_position(position),
        m_offset(offset),
        m_visible(true),
        m_visibleWhen(nullptr),
        m_masterId(-1)
    { }

    virtual ~MenuElement()
    { }

    virtual void update(const sf::RenderWindow& screen)
    { }
    virtual void draw(const DrawParameter& params) override = 0;
    void setPosition(const sf::Vector2f& position)
    {
        if(position != m_position)
        {
            m_position = position;
            onPositionChanged();
        }
    }

    void setVisible(const bool visible)
    {
        m_visible = visible;
    }

    void setPosition(const float x, const float y)
    {
        setPosition(sf::Vector2f(x, y));
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
        return m_position;
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

protected:

    virtual void onPositionChanged()
    { }

    const sf::Vector2f& getOffset()
    {
        return m_offset;
    }

private:

    int m_id;
    bool m_visible;
    int m_masterId;
    const MenuElement* m_visibleWhen;
    MenuElementType::Type m_type;
    sf::Vector2f m_position;
    sf::Vector2f m_offset;
};

#endif // MENU_ELEMENT_HPP
