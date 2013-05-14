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
public:
    
    Clamp(std::vector<std::unique_ptr<ValueProvider>>& provider) : MultiProvider(std::move(provider))
    {
        if(getProvider().size() != 3)
            throw std::runtime_error(utility::replace(utility::translateKey("ThreeChilds"), "Clamp"));
    }

    virtual float getValue()
    {
        float actual = getProvider()[0]->getValue();
        float minv = getProvider()[1]->getValue();
        float maxv = getProvider()[2]->getValue();
        return std::min(std::max(actual, minv), maxv);
    }
};

#endif //CLAMP_HPP
