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
            throw std::runtime_error(utility::replace(utility::translateKey("TwoChilds"), "Modulo"));
    }

    virtual float getValue() override
    {
        float dividend = getProvider()[0]->getValue();
        float divisor = getProvider()[1]->getValue();
        return fmodf(dividend, divisor);
    }

    virtual Modulo* clone() const override
    {
        auto list = std::move(cloneProviders());
        return new Modulo(list);
    }
};

#endif //MODULO_HPP
