#pragma once

#ifndef SET_VARIABLE_HPP
#define SET_VARIABLE_HPP

#include "../VariableHandler.hpp"
#include "SingleProvider.hpp"

#include <exception>
#include <iostream>
#include <memory>
#include <string>

/// Sets the value of a variable owned by someone else and returns the child-value.
class SetVariable : public SingleProvider
{
private:

    VariableHandler* m_owner;
    std::string m_varName;
    bool m_print;

public:
    SetVariable(
        VariableHandler* owner,
        const std::string& varName,
        std::unique_ptr<ValueProvider> provider,
        bool print = false) :
            SingleProvider(std::move(provider)),
            m_owner(owner),
            m_varName(varName),
            m_print(print)
    {
        if(owner == nullptr)
            throw std::runtime_error(utility::replace(utility::translateKey("OwnerNull"), "SetVariable-provider"));
    }

    virtual float getValue()
    {
        float value = getProvider()->getValue();
        m_owner->setValueOf(m_varName, value);
        if(m_print)
            std::cout << m_varName << "=" << value << std::endl;
        return value;
    }
};

#endif //SET_VARIABLE_HPP
