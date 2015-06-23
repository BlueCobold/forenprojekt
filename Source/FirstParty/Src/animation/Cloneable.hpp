#pragma once

#ifndef CLONEABLE_HPP
#define CLONEABLE_HPP

#include <memory>

template<typename T>
class Cloneable
{
public:
    virtual std::unique_ptr<T> clone() const = 0;
};

#endif // CLONEABLE_HPP
