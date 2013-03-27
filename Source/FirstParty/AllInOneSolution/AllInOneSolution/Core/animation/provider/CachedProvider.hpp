#pragma once

#ifndef CACHED_PROVIDER_HPP
#define CACHED_PROVIDER_HPP

#include "SingleProvider.hpp"
#include "ValueProvider.hpp"

#include <memory>
#include <cmath>

/// Returns the value of the passed provider by calling it only once and then storing the value
class CachedProvider : public SingleProvider
{
public:
    
    CachedProvider(std::unique_ptr<ValueProvider> provider) :
        SingleProvider(std::move(provider)),
        m_inited(false)
    { }

    virtual float getValue()
    {
        if(!m_inited)
        {
            m_value = getProvider()->getValue();
            m_inited = true;
        }
        return m_value;
    }

private:

    bool m_inited;
    float m_value;
};

#endif //CACHED_PROVIDER_HPP
