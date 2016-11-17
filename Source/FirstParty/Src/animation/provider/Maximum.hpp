#pragma once

#ifndef MAXIMUM_HPP
#define MAXIMUM_HPP

#include "MultiProvider.hpp"

#include <algorithm>
#include <exception>
#include <memory>
#include <vector>

/// Returns the largest of the values of the passed providers
class Maximum : public MultiProvider
{
    std::unique_ptr<ValueProvider> doClone() const override
    {
        auto list = cloneProviders();
        return std::unique_ptr<Maximum>(new Maximum(std::move(list)));
    }
    
    double calculateValue() override
    {
        auto v = 0.0;
        bool set = false;
        for(auto it = begin(getProvider()); it != end(getProvider()); ++it)
            if(!set)
            {
                v = (*it)->getValue();
                set = true;
            }
            else
                v = std::max(v, (*it)->getValue());
        return v;
    }

public:

    Maximum(std::vector<std::unique_ptr<ValueProvider>> provider) : MultiProvider(std::move(provider))
    {
       if(getProvider().size() < 1)
           throw std::runtime_error(utility::replace(utility::translateKey("@TwoChildsMin"), "Maximum"));
    }
};

#endif //MAXIMUM_HPP
