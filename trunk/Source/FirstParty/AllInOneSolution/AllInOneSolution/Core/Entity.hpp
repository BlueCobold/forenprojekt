#pragma once

#include "PhysicalObject.hpp"
#include "GraphicalObject.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


class Entity : public sf::Drawable , public PhysicalObject, public GraphicalObject
{
public:

    void update();

private:

    void draw(sf::RenderTarget& target, sf::RenderStates states)const;

};

