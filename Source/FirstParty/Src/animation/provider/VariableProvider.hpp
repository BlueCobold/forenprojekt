#pragma once

#ifndef VARIABLE_PROVIDER_HPP
#define VARIABLE_PROVIDER_HPP

#include "Observer.hpp"
#include "ValueProvider.hpp"
#include "../VariableHandler.hpp"

#include <exception>
#include <string>

/// Returns the value of a variable owned by someone else.
class VariableProvider : public ValueProvider, public Observer<const VariableHandler>
{
    std::string m_varName;

    std::unique_ptr<ValueProvider> doClone() const override
    {
        return std::unique_ptr<VariableProvider>(new VariableProvider(getCloneObservable(), m_varName, getCallback()));
    }

    double calculateValue() override
    {
        return getObserved().getValueOf(m_varName);
    }

public:
    VariableProvider(const VariableHandler& observed, const std::string& varName, const CloneCallback cloneCallback = nullptr) :
        Observer(observed, cloneCallback),
        m_varName(varName)
    { }
};

#endif //VARIABLE_PROVIDER_HPP
