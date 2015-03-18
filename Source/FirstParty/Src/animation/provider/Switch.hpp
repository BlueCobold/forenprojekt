#pragma once

#ifndef SWITCH_HPP
#define SWITCH_HPP

#include "MultiProvider.hpp"

#include <exception>
#include <memory>
#include <vector>

/// Returns the value of the providers that is equal to a case
class Switch : public MultiProvider
{
public:

    Switch(std::vector<std::unique_ptr<ValueProvider>>&& provider, const std::vector<float>& constants)
       : MultiProvider(std::move(provider)),
         m_constants(constants)
    {
        if(getProvider().size() < 1)
            throw std::runtime_error(utility::replace(utility::translateKey("TwoChilds"), "Switch"));
        if(getProvider().size() + 1 < constants.size())
            throw std::runtime_error(utility::translateKey("SwitchInvalidSize"));
    }

    virtual float getValue() override
    {
        auto it = begin(getProvider());
        auto current = (*it)->getValue();
        ++it;
        for(auto cit = begin(m_constants); it != end(getProvider()); ++it, ++cit)
        {
            if(current <= (*cit))
                return (*it)->getValue();
        }
        return 0;
    }

    virtual Switch* clone() const override
    {
        auto list = cloneProviders();
        return new Switch(std::move(list), m_constants);
    }

private:
    std::vector<float> m_constants;
};

#endif //SWITCH_HPP
