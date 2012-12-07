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
            throw std::runtime_error("Add needs two at least one child to function.");
    }

    virtual float getValue() const
    {
        float v = 1.0f;
        for(auto it = getProvider().begin(); it != getProvider().end(); ++it)
            v += (*it)->getValue();
        return v;
    }
};

#endif //ADDER_HPP
