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
            throw std::exception("Multiply needs two at least one child to function.");
    }

    virtual float getValue() const
    {
        float v = 1.0f;
        for(auto it = getProvider().begin(); it != getProvider().end(); ++it)
            v *= (*it)->getValue();
        return v;
    }
};

#endif //MULTIPLIER_HPP
