#pragma once

#ifndef VARIABLE_HOLDER_HPP
#define VARIABLE_HOLDER_HPP

#include "provider/ValueProvider.hpp"
#include "provider/StaticProvider.hpp"
#include "VariableHandler.hpp"

#include <exception>
#include <map>
#include <string>

#include "../Utility.hpp"

/// This class offers a basic implementation for a simple VariableHandler
class VariableHolder : public VariableHandler
{
    std::map<std::string, double> m_variables;
    
    double onGetValueOf(const std::string& name) const override
    {
        auto found = m_variables.find(name);
        if(found != end(m_variables))
            return found->second;
        else
            throw std::runtime_error(utility::replace(utility::translateKey("@NoVariable"), name));
        return 0;
    }

    void onSetValueOf(const std::string& name, const double value) override
    {
        m_variables[name] = value;
    }
    
public:
    void copyValuesFrom(const VariableHolder& other)
    {
        m_variables = other.m_variables;
    }
};

#endif //VARIABLE_HOLDER_HPP
