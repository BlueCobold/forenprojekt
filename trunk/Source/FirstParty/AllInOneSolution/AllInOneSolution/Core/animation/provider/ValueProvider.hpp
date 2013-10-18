#pragma once

#ifndef VALUE_PROVIDER_HPP
#define VALUE_PROVIDER_HPP

#include "../Cloneable.hpp"

/// Generates or provides specific values.
class ValueProvider : public Cloneable
{
public:

    virtual ~ValueProvider()
    { }

    virtual float getValue() = 0;

    virtual void reset()
    { }

    virtual ValueProvider* clone() const = 0;
};

#endif //VALUE_PROVIDER_HPP
