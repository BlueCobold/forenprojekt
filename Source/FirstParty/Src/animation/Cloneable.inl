
#include "Cloneable.hpp"

template <typename T>
inline std::unique_ptr<T> Cloneable<T>::clone() const
{
    return doClone();
}