#pragma once

#ifndef VALUE_PROVIDER_HPP
#define VALUE_PROVIDER_HPP

#include "../Cloneable.hpp"

/// Generates or provides specific values.
class ValueProvider : public Cloneable<ValueProvider>
{
public:

    virtual ~ValueProvider()
    { }

    virtual float getValue() = 0;

    virtual void reset()
    { }
};

#endif //VALUE_PROVIDER_HPP
