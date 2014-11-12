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
public:

    Minimum(std::vector<std::unique_ptr<ValueProvider>>&& provider) : MultiProvider(std::move(provider))
    {
       if(getProvider().size() < 1)
           throw std::runtime_error(utility::replace(utility::translateKey("TwoChildsMin"), "Minimum"));
    }

    virtual float getValue() override
    {
        float v;
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

    virtual Minimum* clone() const override
    {
        auto list = cloneProviders();
        return new Minimum(std::move(list));
    }
};

#endif //MINIMUM_HPP
