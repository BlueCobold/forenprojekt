#pragma once

#ifndef SET_VARIABLE_HPP
#define SET_VARIABLE_HPP

#include "../VariableHandler.hpp"
#include "SingleProvider.hpp"

#include <exception>
#include <memory>
#include <string>

/// Sets the value of a variable owned by someone else and returns the child-value.
class SetVariable : public SingleProvider
{
private:

    VariableHandler* m_owner;
    std::string m_varName;

public:
    SetVariable(VariableHandler* owner, const std::string& varName, std::unique_ptr<ValueProvider> provider)
        : SingleProvider(std::move(provider)),
        m_owner(owner),
        m_varName(varName)
    {
        if(owner == nullptr)
            throw std::exception(utility::replace(utility::translateKey("OwnerNull"), "SetVariable-provider").c_str());
    }

    virtual float getValue()
    {
        float value = getProvider()->getValue();
        m_owner->setValueOf(m_varName, value);
        return value;
    }
};

#endif //SET_VARIABLE_HPP
