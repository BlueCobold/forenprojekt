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
    
    Minimum(std::vector<std::unique_ptr<ValueProvider>>& provider) : MultiProvider(std::move(provider))
    {
        if(getProvider().size() < 1)
            throw std::exception(utility::replace(utility::translateKey("TwoChildsMin"), "Minimum").c_str());
    }

    virtual float getValue()
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
};

#endif //MINIMUM_HPP
