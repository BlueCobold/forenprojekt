#pragma once

#ifndef GRAPHICAL_OBJECT_HPP
#define GRAPHICAL_OBJECT_HPP

#include "../animation/Animation.hpp"
#include "AnimatedGraphics.hpp"
#include "Drawable.hpp"

#include <memory>
#include <vector>

/// This class defines typical behavior of graphical objects
class GraphicalObject : public Drawable
{
private:

    std::vector<std::unique_ptr<Animation>> m_animations;

public:

    virtual void draw(const DrawParameter& param)
    {
        for(auto animation = begin(m_animations); animation != end(m_animations); ++animation)
            if((*animation) != nullptr)
                (*animation)->draw(param);
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