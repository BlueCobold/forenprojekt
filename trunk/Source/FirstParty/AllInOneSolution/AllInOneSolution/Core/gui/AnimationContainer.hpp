#pragma once

#ifndef ANIMATIONCONTAINER_HPP
#define ANIMATIONCONTAINER_HPP

#include "../model/AnimatedGraphics.hpp"
#include "MenuElement.hpp"

class AnimationContainer : public AnimatedGraphics, MenuElement
{
private:
    std::map<std::string, float> m_variables;
    Animation* m_updatingAni;
public:
    AnimationContainer() :
        MenuElement(-1, MenuElementType::Animation, sf::Vector2f(0, 0), sf::Vector2f(0, 0))
    {
    }

    virtual float getValueOf(const std::string& name) const override
    {
        auto match = m_variables.find(name);
        if(match == end(m_variables))
        {
            if(m_updatingAni == nullptr)
                throw std::runtime_error(utility::replace(utility::translateKey("GetVariable"), name));
            return m_updatingAni->getValueOf(name);
        }
        return match->second;
    }

    virtual void setValueOf(const std::string& name, const float value) override
    {
        if(m_updatingAni == nullptr)
        m_variables[name] = value;
        else
        {
            auto match = m_variables.find(name);
            if(match == end(m_variables))
                m_updatingAni->setValueOf(name, value);
            else
                m_variables[name] = value;
        }
    }

    void draw(const DrawParameter& param)
    {
        for(auto animation = begin(getAnimations()); animation != end(getAnimations()); ++animation)
        {
            auto ani = (*animation).get();
            if(ani->isStopped())
                continue;
            m_updatingAni = ani;
            ani->setPosition(utility::toPixel(getPosition().x), utility::toPixel(getPosition().y));
            ani->update();
        }
    }
};

#endif