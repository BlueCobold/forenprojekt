#pragma once

#ifndef NOP_HPP
#define NOP_HPP

#include "MultiProvider.hpp"
#include "ValueProvider.hpp"

#include <exception>
#include <memory>
#include <vector>

/// Returns the sum of the values of the passed providers
class Nop : public MultiProvider
{
public:
    
    Nop(std::vector<std::unique_ptr<ValueProvider>>& provider) : MultiProvider(std::move(provider))
    {
        if(getProvider().size() < 0)
            throw std::runtime_error(utility::replace(utility::translateKey("OneChildsMin"), "Nop"));
    }

    virtual float getValue()
    {
        float v = 0.0f;
        bool first = true;
        for(auto it = begin(getProvider()); it != end(getProvider()); ++it)
        {
            float val = (*it)->getValue();
            if(first)
            {
                v = val;
                first = false;
            }
        }
        return v;
    }
};

#endif //NOP_HPP
