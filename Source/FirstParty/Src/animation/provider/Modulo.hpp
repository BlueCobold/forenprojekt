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
    std::unique_ptr<ValueProvider> doClone() const override
    {
        auto list = cloneProviders();
        return std::unique_ptr<Modulo>(new Modulo(std::move(list)));
    }
    
    double calculateValue() override
    {
        auto dividend = getProvider()[0]->getValue();
        auto divisor = getProvider()[1]->getValue();
        return fmod(dividend, divisor);
    }

public:
    Modulo(std::vector<std::unique_ptr<ValueProvider>> provider) : MultiProvider(std::move(provider))
    {
       if(getProvider().size() != 2)
           throw std::runtime_error(utility::replace(utility::translateKey("TwoChilds"), "Modulo"));
    }
};

#endif //MODULO_HPP
