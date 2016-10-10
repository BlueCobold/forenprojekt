#pragma once

#ifndef CLONEABLE_HPP
#define CLONEABLE_HPP

#include <memory>

template<typename T>
class Cloneable
{
public:
    std::unique_ptr<T> clone() const;

private:
    virtual std::unique_ptr<T> doClone() const = 0;
};

#include "Cloneable.inl"

#endif // CLONEABLE_HPP
