#pragma once

#ifndef SET_VARIABLE_HPP
#define SET_VARIABLE_HPP

#include "../VariableHandler.hpp"
#include "SingleProvider.hpp"
#include "Observer.hpp"

#include <exception>
#include <iostream>
#include <memory>
#include <string>

/// Sets the value of a variable owned by someone else and returns the child-value.
class SetVariable : public SingleProvider, public Observer<VariableHandler>
{
private:

    std::string m_varName;
    bool m_print;

public:
    SetVariable(
        VariableHandler& observed,
        const std::string& varName,
        std::unique_ptr<ValueProvider> provider,
        bool print = false,
        const CloneCallback cloneCallback = nullptr) :
            SingleProvider(std::move(provider)),
            Observer(observed, cloneCallback),
            m_varName(varName),
            m_print(print)
    { }

    virtual float getValue() override
    {
        float value = getProvider()->getValue();
        getObserved().setValueOf(m_varName, value);
        if(m_print)
            std::cout << m_varName << "=" << value << std::endl;
        return value;
    }

    virtual std::unique_ptr<ValueProvider> clone() const override
    {
        return std::unique_ptr<SetVariable>(new SetVariable(getCloneObservable(), m_varName,
            getProvider()->clone(), m_print, getCallback()));
    }
};

#endif //SET_VARIABLE_HPP
