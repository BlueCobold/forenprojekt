#pragma once

#ifndef VARIABLE_HANDLER_HPP
#define VARIABLE_HANDLER_HPP

#include <string>

/// This interface specifies an object that can set and return values of variables.
class VariableHandler
{
public:

    double getValueOf(const std::string& name) const
    {
        return onGetValueOf(name);
    }

    void setValueOf(const std::string& name, const double value)
    {
        onSetValueOf(name, value);
    }

protected:
    virtual double onGetValueOf(const std::string& name) const = 0;
    virtual void onSetValueOf(const std::string& name, const double value) = 0;
};

#endif //VARIABLE_HANDLER_HPP
