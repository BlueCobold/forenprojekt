#pragma once

#ifndef ADDER_HPP
#define ADDER_HPP

#include "MultiProvider.hpp"

#include <exception>
#include <memory>
#include <vector>

/// Returns the sum of the values of the passed providers
class Adder : public MultiProvider
{
    std::unique_ptr<ValueProvider> doClone() const override
    {
        auto list = cloneProviders();
        return std::unique_ptr<Adder>(new Adder(std::move(list)));
    }
    
    float calculateValue() override
    {
        float v = 0.0f;
        for(auto it = begin(getProvider()); it != end(getProvider()); ++it)
            v += (*it)->getValue();
        return v;
    }

public:
    Adder(std::vector<std::unique_ptr<ValueProvider>> provider) : MultiProvider(std::move(provider))
    {
       if(getProvider().size() < 1)
           throw std::runtime_error(utility::replace(utility::translateKey("TwoChilds"), "Add"));
    }
};

#endif //ADDER_HPP
