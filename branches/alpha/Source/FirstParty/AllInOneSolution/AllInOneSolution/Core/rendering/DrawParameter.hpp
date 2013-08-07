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
        auto view = m_target.getView();
        return sf::Rect<float>(view.getCenter().x - view.getSize().x / 2.f,
                               view.getCenter().y - view.getSize().y / 2.f,
                               static_cast<float>(view.getSize().x),
                               static_cast<float>(view.getSize().y));
    }

private:
    sf::RenderTarget& m_target;
};

#endif // DRAW_PARAMETER_HPP
