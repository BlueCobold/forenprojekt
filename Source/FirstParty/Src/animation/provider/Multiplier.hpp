#pragma once

#ifndef MULTIPLIER_HPP
#define MULTIPLIER_HPP

#include "MultiProvider.hpp"

#include <memory>
#include <vector>

/// Returns the product of the values of the passed providers
class Multiplier : public MultiProvider
{
    std::unique_ptr<ValueProvider> doClone() const override
    {
        auto list = cloneProviders();
        return std::unique_ptr<Multiplier>(new Multiplier(std::move(list)));
    }

    float calculateValue() override
    {
        float v = 1.0f;
        for(auto it = begin(getProvider()); it != end(getProvider()); ++it)
            v *= (*it)->getValue();
        return v;
    }

public:
    Multiplier(std::vector<std::unique_ptr<ValueProvider>> provider) : MultiProvider(std::move(provider))
    {
       if(getProvider().size() < 1)
           throw std::runtime_error(utility::replace(utility::translateKey("TwoChildsMin"), "Multiply"));
    }
    
};

#endif //MULTIPLIER_HPP
