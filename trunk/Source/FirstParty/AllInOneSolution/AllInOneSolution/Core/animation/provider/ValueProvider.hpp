#pragma once

#ifndef VALUE_PROVIDER_HPP
#define VALUE_PROVIDER_HPP

/// Generates or provides specific values.
class ValueProvider
{
public:

    virtual ~ValueProvider()
    { }

    virtual float getValue() = 0;
};

#endif //VALUE_PROVIDER_HPP
