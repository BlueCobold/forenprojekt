#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

class Entity : public sf::Drawable
{
public:

    Entity();
    virtual ~Entity();

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

protected:

    sf::Texture m_texture;
    sf::Transform m_transform;

};

