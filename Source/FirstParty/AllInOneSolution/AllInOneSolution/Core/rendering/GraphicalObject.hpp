#pragma once

#ifndef GRAPHICAL_OBJECT_HPP
#define GRAPHICAL_OBJECT_HPP

#include "../animation/Animation.hpp"
#include "../model/AnimatedGraphics.hpp"
#include "Drawable.hpp"

#include <memory>
#include <vector>

/// This class defines typical behavior of graphical objects
class GraphicalObject : public Drawable
{
private:

    std::vector<std::unique_ptr<Animation>> m_animations;
    bool m_hide;
    float m_z;

public:

    GraphicalObject() : 
        m_hide(false),
        m_z(0)
    {
    }

    GraphicalObject(const GraphicalObject& rhs)
    {
        m_hide = rhs.m_hide;
        for(auto it = std::begin(rhs.m_animations); it != std::end(rhs.m_animations); ++it)
            m_animations.push_back(std::unique_ptr<Animation>(it->get()->clone()));
    }

    GraphicalObject& operator=(const GraphicalObject& rhs)
    {
        m_hide = rhs.m_hide;
        for(auto it = std::begin(rhs.m_animations); it != std::end(rhs.m_animations); ++it)
            m_animations.push_back(std::unique_ptr<Animation>(it->get()->clone()));

        return *this;
    }

    void hide()
    {
        m_hide = true;
    }

    void unhide()
    {
        m_hide = false;
    }

    bool hidden() const
    {
        return m_hide;
    }

    virtual void draw(const DrawParameter& param)
    {
        if(m_hide)
            return;

        for(auto animation = begin(m_animations); animation != end(m_animations); ++animation)
            if((*animation) != nullptr && !(*animation)->isStopped())
                (*animation)->draw(param);
    }

    void bindAnimation(std::unique_ptr<Animation> animation)
    {
        m_animations.push_back(std::move(animation));
    }

    void setDrawOrder(const float drawOrder)
    {
        m_z = drawOrder;
    }

    float getDrawOrder() const
    {
        return m_z;
    }

protected:
    const std::vector<std::unique_ptr<Animation>>& getAnimations() const
    {
        return m_animations;
    }
};

#endif // GRAPHICAL_OBJECT_HPP
