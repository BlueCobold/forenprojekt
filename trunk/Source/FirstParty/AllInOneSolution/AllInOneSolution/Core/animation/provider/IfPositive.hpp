#pragma once

#ifndef IF_POSITIVE_HPP
#define IF_POSITIVE_HPP

#include "MultiProvider.hpp"
#include "ValueProvider.hpp"

#include <cmath>
#include <exception>
#include <memory>

/// Returns provider 1, if provider 1 >= 0, else provider 2
class IfPositive : public MultiProvider
{
public:
  
    IfPositive(std::vector<std::unique_ptr<ValueProvider>>&& provider) : MultiProvider(std::move(provider))
   {
       if(getProvider().size() != 2)
           throw std::runtime_error(utility::replace(utility::translateKey("TwoChilds"), "IfPositive"));
    }

    virtual float getValue() override
    {
        float value = getProvider()[0]->getValue();
        if(value >= 0)
            return value;
        else
            return getProvider()[1]->getValue();
    }

   virtual IfPositive* clone() const override
   {
        auto list = cloneProviders();
        return new IfPositive(std::move(list));
   }
};

#endif //IF_POSITIVE_HPP
