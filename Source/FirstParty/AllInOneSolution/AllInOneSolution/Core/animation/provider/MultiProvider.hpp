#pragma once

#ifndef MULTI_PROVIDER_HPP
#define MULTI_PROVIDER_HPP

#include "ValueProvider.hpp"

#include <memory>
#include <vector>

/// Returns the sum of the values of the passed providers
class MultiProvider : public ValueProvider
{
private:

    std::vector<std::unique_ptr<ValueProvider>> m_provider;

public:

    MultiProvider(std::vector<std::unique_ptr<ValueProvider>>& provider) : m_provider(std::move(provider))
    {
        for(auto it = begin(m_provider); it != end(m_provider); ++it)
            if((*it) == nullptr)
                throw std::exception("One of the providers has been passed incorrectly and is null.");
    }

    virtual float getValue() const = 0;

protected:

    const std::vector<std::unique_ptr<ValueProvider>>& getProvider() const
    {
        return m_provider;
    }
};

#endif //MULTI_PROVIDER_HPP
