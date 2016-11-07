#pragma once

#ifndef COUNT_PROVIDER_HPP
#define COUNT_PROVIDER_HPP

#include "ValueProvider.hpp"

/// Returns start+x*increment, where x is the number of calls to "getValue"
class Count : public ValueProvider
{
    double m_start;
    double m_increment;
    int m_calls;

    std::unique_ptr<ValueProvider> doClone() const override
    {
        return std::unique_ptr<Count>(new Count(m_start, m_increment));
    }

    double calculateValue() override
    {
        return m_start + (m_calls++) * m_increment;
    }

    void onReset() override
    {
        m_calls = 0;
    }

public:
    Count(const double start, const double increment) :
        m_start(start),
        m_increment(increment),
        m_calls(0)
    { }
};

#endif //COUNT_PROVIDER_HPP
