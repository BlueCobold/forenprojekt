#pragma once

#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include "../TimedObject.hpp"
#include "../../Utility.hpp"

#include <cmath>
#include <functional>

template<typename O>
class Observer
{
public:

    typedef const Observer<O> CallbackParam;
    typedef std::function<O&(CallbackParam&)> CloneCallback;

    Observer(O& observed, const CloneCallback callback = nullptr) :
        m_observed(observed),
        m_cloneCallback(callback)
    {
        if(&observed == nullptr)
            throw std::runtime_error(utility::replace(utility::translateKey("OwnerNull"), "ObserverProvider"));
    }

    O& getObserved() const
    {
        return m_observed;
    }

protected:

    O& getCloneObservable() const
    {
        if(m_cloneCallback)
            return m_cloneCallback(*this);
        return m_observed;
    }

    const CloneCallback getCallback() const
    {
        return m_cloneCallback;
    }

private:

    O& m_observed;
    const CloneCallback m_cloneCallback;
};

#endif //OBSERVER_HPP
