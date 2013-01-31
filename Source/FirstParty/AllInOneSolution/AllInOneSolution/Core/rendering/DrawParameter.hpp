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

private:
    sf::RenderTarget& m_target;
};

#endif // DRAW_PARAMETER_HPP
