#pragma once

#ifndef TEST_HPP
#define TEST_HPP

#include "State.hpp"
#include "Animation.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

class Test : public State
{
public:

    Test(sf::RenderWindow& screen);
    ~Test();

    bool initialize();

    void update();

    void draw();
    
    void shutdown();

private:

    sf::Clock m_clock;
    sf::Texture m_texture;
    Animation m_animation;
    
};

#endif // TEST_HPP
