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
    double onGetValueOf(const std::string& name) const override;
    void onSetValueOf(const std::string& name, const double value) override;
    
    std::map<std::string, double> m_variables;
    sf::Clock m_clock;
};

#endif
