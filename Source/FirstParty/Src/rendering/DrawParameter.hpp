#pragma once

#ifndef DRAW_PARAMETER_HPP
#define DRAW_PARAMETER_HPP

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../Utility.hpp"

#include <functional>
#include <vector>
#include <unordered_map>

class DrawParameter
{
public:
    DrawParameter(sf::RenderTexture& target) :
        m_primaryTextureTarget(&target),
        m_primaryWindowTarget(nullptr)
    {
        m_buffers.push_back(&target);
    }

    DrawParameter(sf::RenderWindow& target) :
        m_primaryTextureTarget(nullptr),
        m_primaryWindowTarget(&target)
    {
        m_buffers.push_back(&target);
    }

    sf::RenderTarget& getTarget() const
    {
        return *m_buffers[0];
    }
    
    void activatePrimary() const
    {
        if(m_primaryTextureTarget)
            m_primaryTextureTarget->setActive(true);
        else if(m_primaryWindowTarget)
            m_primaryWindowTarget->setActive(true);
    }

    sf::RenderTarget& getTarget(unsigned int bufferLayer) const
    {
        if(bufferLayer > m_buffers.size())
            throw std::runtime_error(utility::translateKey("@UnknownLayer"));

        auto buffer = m_buffers[bufferLayer];
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
        auto view = m_buffers[0]->getView();
        return sf::Rect<float>(view.getCenter().x - view.getSize().x / 2.f,
                               view.getCenter().y - view.getSize().y / 2.f,
                               static_cast<float>(view.getSize().x),
                               static_cast<float>(view.getSize().y));
    }

    void addTargetBuffer(sf::RenderTarget& buffer)
    {
        m_buffers.push_back(&buffer);
    }

    int getOffscreenCount() const
    {
        return static_cast<int>(m_buffers.size()) - 1;
    }

private:
    std::function<void(const sf::Texture*)> m_textureCallback;
    std::vector<sf::RenderTarget*> m_buffers;
    sf::RenderTexture* m_primaryTextureTarget;
    sf::RenderWindow* m_primaryWindowTarget;
};

#endif // DRAW_PARAMETER_HPP
