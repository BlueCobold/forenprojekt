#pragma once

#ifndef SUBSTRACTOR_HPP
#define SUBSTRACTOR_HPP

#include "MultiProvider.hpp"
#include "ValueProvider.hpp"

#include <exception>
#include <memory>
#include <vector>

/// Returns the difference of the values of the passed providers
class Substractor : public MultiProvider
{
public:
    
    Substractor(std::vector<std::unique_ptr<ValueProvider>>& provider) : MultiProvider(std::move(provider))
    {
        if(getProvider().size() < 1)
            throw std::runtime_error(utility::replace(utility::translateKey("TwoChildsMin"), "Sub"));
    }

    virtual float getValue()
    {
        float v = 0.0f;
        bool first = true;
        for(auto it = begin(getProvider()); it != end(getProvider()); ++it)
        {
            if(first)
                v = (*it)->getValue();
            else
                v -= (*it)->getValue();
            first = false;
        }
        return v;
    }
};

#endif //SUBSTRACTOR_HPP
