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
    CachedProvider(std::unique_ptr<ValueProvider> provider, const bool persistent = false) :
        SingleProvider(std::move(provider)),
        m_inited(false),
        m_persistent(persistent)
    { }

private:
    double calculateValue() override
    {
        if(!m_inited)
        {
            m_value = getProvider()->getValue();
            m_inited = true;
        }
        return m_value;
    }

    void onReset() override
    {
        if(!m_persistent)
            m_inited = false;
    }

    std::unique_ptr<ValueProvider> doClone() const override
    {
        return std::unique_ptr<CachedProvider>(new CachedProvider(getProvider()->clone(), m_persistent));
    }

    bool m_inited;
    bool m_persistent;
    double m_value;
};

#endif //CACHED_PROVIDER_HPP
