#pragma once

#ifndef STEP_HPP
#define STEP_HPP

#include "MultiProvider.hpp"
#include "ValueProvider.hpp"

#include <cmath>
#include <exception>
#include <memory>

/// Returns provider 2, if provider 1 < 0, else provider 3
class Step : public MultiProvider
{
public:
    
    Step(std::vector<std::unique_ptr<ValueProvider>>& provider) : MultiProvider(std::move(provider))
    {
        if(getProvider().size() != 3)
            throw std::exception("Step needs three children to function.");
    }

    virtual float getValue()
    {
        if(getProvider()[0]->getValue() < 1)
            return getProvider()[1]->getValue();
        else
            return getProvider()[2]->getValue();
    }
};

#endif //STEP_HPP
