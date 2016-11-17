#pragma once

#ifndef MULTI_PROVIDER_HPP
#define MULTI_PROVIDER_HPP

#include "ValueProvider.hpp"

#include <memory>
#include <vector>
#include "../../Utility.hpp"

/// Returns some operation performed on multiple providers
class MultiProvider : public ValueProvider
{
    std::vector<std::unique_ptr<ValueProvider>> m_provider;

    void onReset() override
    {
        for(auto it = begin(m_provider); it != end(m_provider); ++it)
            (*it)->reset();
    }

public:

    MultiProvider(std::vector<std::unique_ptr<ValueProvider>> provider) : m_provider(std::move(provider))
    {
       for(auto it = begin(m_provider); it != end(m_provider); ++it)
           if((*it) == nullptr)
                throw std::runtime_error(utility::translateKey("@ProviderNull"));
    }

protected:

    const std::vector<std::unique_ptr<ValueProvider>>& getProvider() const
    {
        return m_provider;
    }

    std::vector<std::unique_ptr<ValueProvider>> cloneProviders() const
    {
        std::vector<std::unique_ptr<ValueProvider>> list(m_provider.size());
        for(unsigned int i = 0; i < m_provider.size(); i++)
            list[i] = m_provider[i]->clone();
        return std::move(list);
    }
};

#endif //MULTI_PROVIDER_HPP
