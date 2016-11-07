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

    double getValue()
    {
        return calculateValue();
    }

    void reset()
    {
        onReset();
    }

private:
    virtual double calculateValue() = 0;

    virtual void onReset()
    { }
};

#endif //VALUE_PROVIDER_HPP
