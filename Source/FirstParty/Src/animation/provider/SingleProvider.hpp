#pragma once

#ifndef SINGLE_PROVIDER_HPP
#define SINGLE_PROVIDER_HPP

#include "ValueProvider.hpp"

#include <exception>
#include <memory>

/// Returns a modified value of the passed providers
class SingleProvider : public ValueProvider
{
private:

    std::unique_ptr<ValueProvider> m_provider;

public:

    SingleProvider(std::unique_ptr<ValueProvider> provider) : m_provider(std::move(provider))
    {
        if(m_provider == nullptr)
            throw std::runtime_error(utility::translateKey("TheProviderNull"));
    }

    virtual void reset() override
    {
        m_provider->reset();
    }

protected:

    ValueProvider* getProvider() const
    {
        return m_provider.get();
    }
};

#endif //SINGLE_PROVIDER_HPP
