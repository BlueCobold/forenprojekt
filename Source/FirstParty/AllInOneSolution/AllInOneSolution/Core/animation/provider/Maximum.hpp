#pragma once

#ifndef MAXIMUM_HPP
#define MAXIMUM_HPP

#include "MultiProvider.hpp"

#include <algorithm>
#include <exception>
#include <memory>
#include <vector>

/// Returns the smallest of the values of the passed providers
class Maximum : public MultiProvider
{
public:

    Maximum(std::vector<std::unique_ptr<ValueProvider>>& provider) : MultiProvider(std::move(provider))
    {
        if(getProvider().size() < 1)
            throw std::exception("Maximum needs two at least one child to function.");
    }

    virtual float getValue() const
    {
        float v;
        bool set = false;
        for(auto it = begin(getProvider()); it != end(getProvider()); ++it)
            if(!set)
            {
                v = (*it)->getValue();
                set = true;
            }
            else
                v = std::max(v, (*it)->getValue());
        return v;
    }
};

#endif //MAXIMUM_HPP
