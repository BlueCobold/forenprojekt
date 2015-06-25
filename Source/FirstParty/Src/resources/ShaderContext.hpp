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

    virtual float getValueOf(const std::string& name) const override;
    virtual void setValueOf(const std::string& name, const float value) override;

    void update();

private:
    
    std::map<std::string, float> m_variables;
    sf::Clock m_clock;
};

#endif
