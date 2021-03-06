#pragma once

#ifndef GRAPHICAL_OBJECT_HPP
#define GRAPHICAL_OBJECT_HPP

#include "../animation/Animation.hpp"
#include "Drawable.hpp"

#include <functional>
#include <memory>
#include <vector>

/// This class defines typical behavior of graphical objects
class GraphicalObject : public Drawable
{
    std::vector<std::unique_ptr<Animation>> m_animations;
    bool m_hide;
    float m_z;

protected:
    void doDraw(const DrawParameter& param) override;

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

    void bindAnimation(std::unique_ptr<Animation> animation);
    typedef std::function<bool(Animation&)> BeforeCallback;
    typedef std::function<void(Animation&)> AfterCallback;
    void updateAnimations(BeforeCallback before = nullptr, AfterCallback after = nullptr) const;

    void setDrawOrder(const float drawOrder);
    float getDrawOrder() const;
};

#endif // GRAPHICAL_OBJECT_HPP
