#pragma once

#ifndef MOUSE_PROVIDER_HPP
#define MOUSE_PROVIDER_HPP

#include "ValueProvider.hpp"
#include "../../Input.hpp"

#include <SFML/Window/Mouse.hpp>

/// Returns the relative mouse coordinates since the last call
class MouseProvider : public ValueProvider
{
    bool m_firstCall;
    bool m_xAxis;
    sf::Vector2f m_lastMousePos;

    std::unique_ptr<ValueProvider> doClone() const override
    {
        return std::unique_ptr<MouseProvider>(new MouseProvider(m_xAxis));
    }

public:
    
    MouseProvider(bool xAxis)
      : m_firstCall(true),
        m_xAxis(xAxis)
    { }

    float getValue() override
    {
        if(m_firstCall)
        {
            m_firstCall = false;
            m_lastMousePos = utility::Mouse.getPosition();
            return 0;
        }
        auto mouseDiff = (m_lastMousePos - utility::Mouse.getPosition());
        m_lastMousePos = utility::Mouse.getPosition();
        if(m_xAxis)
            return mouseDiff.x;
        return mouseDiff.y;
    }

    void reset() override
    {
        m_firstCall = true;
    }
};

#endif //MOUSE_PROVIDER_HPP
