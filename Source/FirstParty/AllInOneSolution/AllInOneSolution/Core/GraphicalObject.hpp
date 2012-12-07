#pragma once

#ifndef GRAPHICAL_OBJECT_HPP
#define GRAPHICAL_OBJECT_HPP

#include "Animation.hpp"

#include <SFML/Graphics/Drawable.hpp>

#include <memory>

/// This class defines typical behavior of graphical objects
class GraphicalObject : public sf::Drawable
{
private:

    std::unique_ptr<Animation> m_animation;

public:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if(m_animation != nullptr)
            m_animation->draw(target, states);
    }

    void bindAnimation(std::unique_ptr<Animation> animation)
    {
        m_animation = std::move(animation);
    }

protected:

    Animation* getAnimation() const
    {
        return m_animation.get();
    }
};

#endif // GRAPHICAL_OBJECT_HPP
