#pragma once

#ifndef MULTIPLIER_HPP
#define MULTIPLIER_HPP

#include "MultiProvider.hpp"

#include <memory>
#include <vector>

/// Returns the product of the values of the passed providers
class Multiplier : public MultiProvider
{
public:
    
    Multiplier(std::vector<std::unique_ptr<ValueProvider>>& provider) : MultiProvider(std::move(provider))
    {
        if(getProvider().size() < 1)
            throw std::exception("Multiply needs at least two children to function.");
    }

    virtual float getValue()
    {
        float v = 1.0f;
        for(auto it = begin(getProvider()); it != end(getProvider()); ++it)
            v *= (*it)->getValue();
        return v;
    }
};

#endif //MULTIPLIER_HPP
