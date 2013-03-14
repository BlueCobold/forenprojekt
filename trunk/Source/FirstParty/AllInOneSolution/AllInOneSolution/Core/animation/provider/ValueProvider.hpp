#pragma once

#ifndef VALUE_PROVIDER_HPP
#define VALUE_PROVIDER_HPP

#include "../../Utility.hpp"

/// Generates or provides specific values.
class ValueProvider
{
public:

    virtual ~ValueProvider()
    { }

    virtual float getValue() = 0;
};

#endif //VALUE_PROVIDER_HPP
