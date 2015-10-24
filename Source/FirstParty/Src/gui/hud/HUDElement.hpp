#ifndef HUDELEMENT_HPP
#define HUDELEMENT_HPP

//#include <SFML/System/Vector2.hpp>
#include "../ScreenLocation.hpp"
#include "../../rendering/Drawable.hpp"

class ResourceManager;

/// Basis class for HUD
class HUDElement : public Drawable
{
private:

    ScreenLocation m_screenLocation;
public:
    HUDElement(const sf::Vector2f& position,
               const sf::Vector2f& offset = sf::Vector2f(ScreenLocation::Left, ScreenLocation::Top));

    virtual ~HUDElement()
    { }

    virtual void update(const DrawParameter& params);

    virtual void draw(const DrawParameter& params) = 0;

    virtual void setPosition(const sf::Vector2f& position, const sf::Vector2f& offset);

protected:
    const sf::Vector2f getCurrentPosition() const;
};

#endif
