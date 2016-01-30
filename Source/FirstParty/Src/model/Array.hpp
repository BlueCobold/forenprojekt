#pragma once

#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <array>

template<typename T, size_t N>
class Array
{
    std::array<T, N> m_storage;

public:
    template<typename S, size_t I>
    class ArrayProxy
    {
        std::array<S, I>& m_target;
        size_t m_index;

    public:
        ArrayProxy(std::array<S, I>& target, size_t index) :
            m_target(target),
            m_index(index)
        { }

        S& operator= (S value)
        {
            m_target[m_index] = std::move(value);
            return m_target[m_index];
        }
        
        S& operator* () const
        {
            return m_target[m_index];
        }
    };

    Array<T, N>()
    { }

    Array<T, N>(Array<T, N>&& other)
    {
        std::move(begin(other.m_storage), end(other.m_storage), begin(m_storage));
    }

    Array<T, N>& operator= (Array<T, N>&& other)
    {
        std::move(begin(other.m_storage), end(other.m_storage), begin(m_storage));
        return *this;
    }

    ArrayProxy<T, N> operator[] (size_t index)
    {
        return ArrayProxy<T, N>(m_storage, index);
    }
};

#endif // ARRAY_HPP
