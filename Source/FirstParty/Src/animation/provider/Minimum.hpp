#pragma once

#ifndef MINIMUM_HPP
#define MINIMUM_HPP

#include "MultiProvider.hpp"

#include <algorithm>
#include <exception>
#include <memory>
#include <vector>

/// Returns the smallest of the values of the passed providers
class Minimum : public MultiProvider
{

    std::unique_ptr<ValueProvider> doClone() const override
    {
        auto list = cloneProviders();
        return std::unique_ptr<Minimum>(new Minimum(std::move(list)));
    }

public:

    Minimum(std::vector<std::unique_ptr<ValueProvider>> provider) : MultiProvider(std::move(provider))
    {
       if(getProvider().size() < 1)
           throw std::runtime_error(utility::replace(utility::translateKey("TwoChildsMin"), "Minimum"));
    }

    float getValue() override
    {
        float v = 0.0f;
        bool set = false;
        for(auto it = begin(getProvider()); it != end(getProvider()); ++it)
            if(!set)
            {
                v = (*it)->getValue();
                set = true;
            }
            else
                v = std::min(v, (*it)->getValue());
        return v;
    }
};

#endif //MINIMUM_HPP
