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
private:
    std::string m_varName;

public:
    VariableProvider(const VariableHandler& observed, const std::string& varName, const CloneCallback cloneCallback = nullptr) :
        Observer(observed, cloneCallback),
        m_varName(varName)
    { }

    virtual float getValue() override
    {
        return getObserved().getValueOf(m_varName);
    }

    virtual std::unique_ptr<ValueProvider> clone() const override
    {
        return std::unique_ptr<VariableProvider>(new VariableProvider(getCloneObservable(), m_varName, getCallback()));
    }
};

#endif //VARIABLE_PROVIDER_HPP
