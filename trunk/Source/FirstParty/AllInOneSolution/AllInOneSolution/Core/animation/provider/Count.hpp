#pragma once

#ifndef COUNT_PROVIDER_HPP
#define COUNT_PROVIDER_HPP

#include "ValueProvider.hpp"

/// Returns start+x*increment, where x is the number of calls to "getValue"
class Count : public ValueProvider
{
private:

    float m_start;
    float m_increment;
    int m_calls;

public:

    Count(const float start, const float increment) :
        m_start(start),
        m_increment(increment),
        m_calls(0)
    { }

    virtual float getValue() override
    {
        return m_start + (m_calls++) * m_increment;
    }

    virtual void reset() override
    {
        m_calls = 0;
    }

    virtual Count* clone() const override
    {
        return new Count(m_start, m_increment);
    }
};

#endif //COUNT_PROVIDER_HPP
