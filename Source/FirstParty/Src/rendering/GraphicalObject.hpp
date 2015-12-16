#pragma once

#ifndef GRAPHICAL_OBJECT_HPP
#define GRAPHICAL_OBJECT_HPP

#include "../animation/Animation.hpp"
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

    GraphicalObject(const GraphicalObject& rhs);
    GraphicalObject& operator=(const GraphicalObject& rhs);

    void hide();
    void unhide();
    bool hidden() const;

    virtual void draw(const DrawParameter& param);

    void bindAnimation(std::unique_ptr<Animation> animation);

    void setDrawOrder(const float drawOrder);
    float getDrawOrder() const;

protected:
    const std::vector<std::unique_ptr<Animation>>& getAnimations() const;
};

#endif // GRAPHICAL_OBJECT_HPP
