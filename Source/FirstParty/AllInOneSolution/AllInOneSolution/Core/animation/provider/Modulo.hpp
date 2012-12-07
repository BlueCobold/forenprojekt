#pragma once

#ifndef MODULO_HPP
#define MODULO_HPP

#include "MultiProvider.hpp"
#include "ValueProvider.hpp"

#include <cmath>
#include <exception>
#include <memory>

/// Returns the the value of provider 1 modulo the value of provider 2
class Modulo : public MultiProvider
{
public:
    
    Modulo(std::vector<std::unique_ptr<ValueProvider>>& provider) : MultiProvider(std::move(provider))
    {
        if(getProvider().size() != 2)
            throw std::runtime_error("Modulo needs two children to function.");
    }

    virtual float getValue() const
    {
        return fmodf(getProvider()[0]->getValue(), getProvider()[1]->getValue());
    }
};

#endif //MODULO_HPP
