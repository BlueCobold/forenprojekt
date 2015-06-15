#pragma once

#ifndef DRAW_PARAMETER_HPP
#define DRAW_PARAMETER_HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "../Utility.hpp"

#include <functional>
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

        auto buffer = m_offscreenBuffers[bufferLayer - 1];

        return *buffer;
    }

    void registerBufferRequest(std::function<void(const sf::Texture*)> callback)
    {
        m_textureCallback = callback;
    }

    void prepareTexture(const sf::Texture* texture) const
    {
        if(m_textureCallback)
            m_textureCallback(texture);
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

    int getOffscreenCount() const
    {
        return m_offscreenBuffers.size();
    }

private:
    sf::RenderTarget& m_target;
    std::function<void(const sf::Texture*)> m_textureCallback;
    std::vector<sf::RenderTexture*> m_offscreenBuffers;
};

#endif // DRAW_PARAMETER_HPP
