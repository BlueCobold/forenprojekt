#pragma once

#ifndef VARIABLE_HOLDER_HPP
#define VARIABLE_HOLDER_HPP

#include "provider/ValueProvider.hpp"
#include "VariableHandler.hpp"

#include <exception>
#include <map>
#include <string>

/// This interface specifies an object that can store and return variables.
class VariableHolder : public VariableHandler
{
private:

    std::map<std::string, std::unique_ptr<ValueProvider>> m_variables;

public:

    virtual float getValueOf(const std::string& name) const
    {
        auto found = m_variables.find(name);
        if(found != m_variables.end())
            return found->second->getValue();
        else
            throw std::runtime_error(std::string("The variable '") + name + std::string("' is not defined."));
        return 0;
    }

    void bindVariables(std::map<std::string, std::unique_ptr<ValueProvider>>& variables)
    {
        m_variables = std::move(variables);
    }
};

#endif //VARIABLE_HOLDER_HPP
