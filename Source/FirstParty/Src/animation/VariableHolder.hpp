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

/// This interface specifies an object that can store and return variables.
class VariableHolder : public virtual VariableHandler
{
private:

    std::map<std::string, float> m_variables;

public:

    virtual float getValueOf(const std::string& name) const override
    {
        auto found = m_variables.find(name);
        if(found != end(m_variables))
            return found->second;
        else
            throw std::runtime_error(utility::replace(utility::translateKey("NoVariable"), name));
        return 0;
    }

    virtual void setValueOf(const std::string& name, const float value) override
    {
        m_variables[name] = value;
    }

    void copyValuesFrom(const VariableHolder& other)
    {
        m_variables = other.m_variables;
    }
};

#endif //VARIABLE_HOLDER_HPP
