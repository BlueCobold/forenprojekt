#pragma once

#ifndef HUDELEMENT_HPP
#define HUDELEMENT_HPP

//#include <SFML/System/Vector2.hpp>
#include "../ScreenLocation.hpp"
#include "../../rendering/Drawable.hpp"

class ResourceManager;

/// Basis class for HUD
class HUDElement : public Drawable
{
    ScreenLocation m_screenLocation;

public:
    HUDElement(const sf::Vector2f& position,
               const sf::Vector2f& offset = sf::Vector2f(ScreenLocation::Left, ScreenLocation::Top));

    virtual ~HUDElement()
    { }

    void update(const DrawParameter& params);
    void setPosition(const sf::Vector2f& position, const sf::Vector2f& offset);

protected:
    const sf::Vector2f getCurrentPosition() const;
    
    virtual void updated(const DrawParameter& params);
    virtual void onPositionSet(const sf::Vector2f& position, const sf::Vector2f& offset);
};

#endif
