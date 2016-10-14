#pragma once

#ifndef NOP_HPP
#define NOP_HPP

#include "MultiProvider.hpp"
#include "ValueProvider.hpp"

#include <exception>
#include <memory>
#include <vector>

/// Executes all passed providers and returns the value of the first
class Nop : public MultiProvider
{
    std::unique_ptr<ValueProvider> doClone() const override
    {
        auto list = cloneProviders();
        return std::unique_ptr<Nop>(new Nop(std::move(list)));
    }
    
    float calculateValue() override
    {
        auto it = begin(getProvider());
        float ret = (*it)->getValue();

        for(++it; it != end(getProvider()); ++it)
            (*it)->getValue();

        return ret;
    }

public:
    Nop(std::vector<std::unique_ptr<ValueProvider>> provider) : MultiProvider(std::move(provider))
    {
       if(getProvider().size() < 1)
           throw std::runtime_error(utility::replace(utility::translateKey("OneChildsMin"), "Nop"));
    }
};

#endif //NOP_HPP
