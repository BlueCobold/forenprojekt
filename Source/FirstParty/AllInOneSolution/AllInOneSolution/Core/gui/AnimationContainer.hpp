#pragma once

#ifndef ANIMATIONCONTAINER_HPP
#define ANIMATIONCONTAINER_HPP

#include "../animation/AnimatedObject.hpp"
#include "MenuElement.hpp"

class AnimationContainer : public AnimatedObject, MenuElement
{
private:
    std::map<std::string, float> m_variables;
public:
    AnimationContainer() :
        MenuElement(-1, MenuElementType::Animation, sf::Vector2f(0, 0), sf::Vector2f(0, 0))
    {
    }

    virtual float getValueOf(const std::string& name) const override
    {
        auto found = m_variables.find(name);
        if(found != end(m_variables))
            return found->second;
        else
            throw std::runtime_error(utility::replace(utility::translateKey("NoVariable"), name));
        return 0;
    }

    virtual void setValueOf(const std::string& name, const float value) override
    {
        m_variables[name] = value;
    }

    void draw(const DrawParameter& param)
    {
    }
};

#endif