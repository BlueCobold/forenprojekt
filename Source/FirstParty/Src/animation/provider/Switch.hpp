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
    std::vector<double> m_constants;

    std::unique_ptr<ValueProvider> doClone() const override
    {
        auto list = cloneProviders();
        return std::unique_ptr<Switch>(new Switch(std::move(list), m_constants));
    }
    
    double calculateValue() override
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

public:
    Switch(std::vector<std::unique_ptr<ValueProvider>> provider, const std::vector<double>& constants)
       : MultiProvider(std::move(provider)),
         m_constants(constants)
    {
        if(getProvider().size() < 1)
            throw std::runtime_error(utility::replace(utility::translateKey("TwoChilds"), "Switch"));
        if(getProvider().size() + 1 < constants.size())
            throw std::runtime_error(utility::translateKey("SwitchInvalidSize"));
    }
};

#endif //SWITCH_HPP
