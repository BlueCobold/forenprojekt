#pragma once

#ifndef VARIABLE_HANDLER_HPP
#define VARIABLE_HANDLER_HPP

#include <string>

/// This interface specifies an object that can return variables.
class VariableHandler
{
public:

    virtual float getValueOf(const std::string& name) const = 0;

    virtual void setValueOf(const std::string& name, const float value) = 0;
};

#endif //VARIABLE_HANDLER_HPP
