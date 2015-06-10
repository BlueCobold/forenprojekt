#pragma once

#ifndef DRAW_PARAMETER_HPP
#define DRAW_PARAMETER_HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "../Utility.hpp"

#include <vector>

class DrawParameter
{
public:
    DrawParameter(sf::RenderTarget& target) :
        m_target(target)
    {
    }

    sf::RenderTarget& getTarget() const
    {
        return m_target;
    }
    
    sf::RenderTarget& getTarget(unsigned int bufferLayer) const
    {
        if(bufferLayer == 0)
            return m_target;
        if(bufferLayer - 1 >= m_offscreenBuffers.size())
            throw std::runtime_error(utility::translateKey("UnknownLayer"));
        return *m_offscreenBuffers[bufferLayer - 1];
    }

    sf::Rect<float> getScreenRect() const
    {
        auto view = m_target.getView();
        return sf::Rect<float>(view.getCenter().x - view.getSize().x / 2.f,
                               view.getCenter().y - view.getSize().y / 2.f,
                               static_cast<float>(view.getSize().x),
                               static_cast<float>(view.getSize().y));
    }

    void addTargetBuffer(sf::RenderTexture& buffer)
    {
        m_offscreenBuffers.push_back(&buffer);
    }

private:
    sf::RenderTarget& m_target;
    std::vector<sf::RenderTexture*> m_offscreenBuffers;
};

#endif // DRAW_PARAMETER_HPP
