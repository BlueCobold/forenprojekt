#pragma once

#ifndef SHADER_CONTEXT_HPP
#define SHADER_CONTEXT_HPP

#include "../animation/AnimatedObject.hpp"

#include <SFML/System/Clock.hpp>

#include <string>
#include <map>

class ShaderContext : public AnimatedObject
{
public:
    void update();

private:
    float onGetValueOf(const std::string& name) const override;
    void onSetValueOf(const std::string& name, const float value) override;
    
    std::map<std::string, float> m_variables;
    sf::Clock m_clock;
};

#endif
