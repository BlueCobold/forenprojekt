#pragma once

#ifndef CLAMP_HPP
#define CLAMP_HPP

#include "MultiProvider.hpp"
#include "ValueProvider.hpp"

#include <algorithm>
#include <exception>
#include <memory>

/// Returns min(max(provider 1, provider 2), provider 3)
class Clamp : public MultiProvider
{
    std::unique_ptr<ValueProvider> doClone() const override
    {
        auto list = cloneProviders();
        return std::unique_ptr<Clamp>(new Clamp(std::move(list)));
    }
    
    double calculateValue() override
    {
        auto actual = getProvider()[0]->getValue();
        auto minv = getProvider()[1]->getValue();
        auto maxv = getProvider()[2]->getValue();
        return std::min(std::max(actual, minv), maxv);
    }

public:
    Clamp(std::vector<std::unique_ptr<ValueProvider>> provider) : MultiProvider(std::move(provider))
    {
       if(getProvider().size() != 3)
           throw std::runtime_error(utility::replace(utility::translateKey("ThreeChilds"), "Clamp"));
    }
};

#endif //CLAMP_HPP
