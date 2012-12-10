#pragma once

#ifndef GRAPHICAL_OBJECT_HPP
#define GRAPHICAL_OBJECT_HPP

#include "Animation.hpp"

#include <SFML/Graphics/Drawable.hpp>

#include <memory>
#include <vector>

/// This class defines typical behavior of graphical objects
class GraphicalObject : public sf::Drawable
{
private:

    std::vector<std::unique_ptr<Animation>> m_animations;

public:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        for(auto animation = m_animations.begin(); animation != m_animations.end(); ++animation)
            if((*animation) != nullptr)
                (*animation)->draw(target, states);
    }

    void bindAnimation(std::unique_ptr<Animation> animation)
    {
        m_animations.push_back(std::move(animation));
    }

protected:

    const std::vector<std::unique_ptr<Animation>>& getAnimations() const
    {
        return m_animations;
    }
};

#endif // GRAPHICAL_OBJECT_HPP
