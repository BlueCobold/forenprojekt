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
public:
  
    Nop(std::vector<std::unique_ptr<ValueProvider>>&& provider) : MultiProvider(std::move(provider))
   {
       if(getProvider().size() < 1)
           throw std::runtime_error(utility::replace(utility::translateKey("OneChildsMin"), "Nop"));
    }

    virtual float getValue() override
    {
        auto it = begin(getProvider());
        float ret = (*it)->getValue();

        for(++it; it != end(getProvider()); ++it)
            (*it)->getValue();

        return ret;
    }

   virtual Nop* clone() const override
   {
        auto list = cloneProviders();
        return new Nop(std::move(list));
   }
};

#endif //NOP_HPP
