#pragma once

#ifndef ADDER_HPP
#define ADDER_HPP

#include "MultiProvider.hpp"
#include "ValueProvider.hpp"

#include <exception>
#include <memory>
#include <vector>

/// Returns the sum of the values of the passed providers
class Adder : public MultiProvider
{
public:
    
    Adder(std::vector<std::unique_ptr<ValueProvider>>& provider) : MultiProvider(std::move(provider))
    {
        if(getProvider().size() < 1)
            throw std::runtime_error(utility::replace(utility::translateKey("TwoChilds"), "Add"));
    }

    virtual float getValue()
    {
        float v = 0.0f;
        for(auto it = begin(getProvider()); it != end(getProvider()); ++it)
            v += (*it)->getValue();
        return v;
    }
};

#endif //ADDER_HPP
