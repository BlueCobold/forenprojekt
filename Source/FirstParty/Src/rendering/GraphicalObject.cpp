
#include "GraphicalObject.hpp"

#include <algorithm>

bool compareDrawOrder(const std::unique_ptr<Animation>& lhs, const std::unique_ptr<Animation>& rhs)
{
    return lhs->getDrawOrder() < rhs->getDrawOrder();
}

GraphicalObject::GraphicalObject(const GraphicalObject& rhs)
{
    m_hide = rhs.m_hide;
    for(auto it = std::begin(rhs.m_animations); it != std::end(rhs.m_animations); ++it)
        m_animations.push_back(it->get()->clone());
}

GraphicalObject& GraphicalObject::operator=(const GraphicalObject& rhs)
{
    m_hide = rhs.m_hide;
    for(auto it = std::begin(rhs.m_animations); it != std::end(rhs.m_animations); ++it)
        m_animations.push_back(it->get()->clone());

    return *this;
}

void GraphicalObject::hide()
{
    m_hide = true;
}

void GraphicalObject::unhide()
{
    m_hide = false;
}

bool GraphicalObject::hidden() const
{
    return m_hide;
}

void GraphicalObject::draw(const DrawParameter& param)
{
    if(m_hide)
        return;

    for(auto animation = begin(m_animations); animation != end(m_animations); ++animation)
        if((*animation) != nullptr && !(*animation)->isStopped())
            (*animation)->draw(param);
}

void GraphicalObject::bindAnimation(std::unique_ptr<Animation> animation)
{
    m_animations.push_back(std::move(animation));
    if(m_animations.size() > 1)
        std::sort(m_animations.begin(), m_animations.end(), compareDrawOrder);
}

void GraphicalObject::setDrawOrder(const float drawOrder)
{
    m_z = drawOrder;
}

float GraphicalObject::getDrawOrder() const
{
    return m_z;
}

const std::vector<std::unique_ptr<Animation>>& GraphicalObject::getAnimations() const
{
    return m_animations;
}
