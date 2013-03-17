#pragma once

#ifndef DRAW_PARAMETER_HPP
#define DRAW_PARAMETER_HPP

#include <SFML/Graphics/RenderTarget.hpp>

class DrawParameter
{
public:
    DrawParameter(sf::RenderTarget& target)
        : m_target(target)
    {
    }

    sf::RenderTarget& getTarget() const
    {
        return m_target;
    }

    sf::Rect<float> getScreenRect() const
    {
        return sf::Rect<float>(m_target.getView().getCenter().x - m_target.getSize().x / 2.f,
                               m_target.getView().getCenter().y - m_target.getSize().y / 2.f,
                               static_cast<float>(m_target.getSize().x),
                               static_cast<float>(m_target.getSize().y));
    }

private:
    sf::RenderTarget& m_target;
};

#endif // DRAW_PARAMETER_HPP
