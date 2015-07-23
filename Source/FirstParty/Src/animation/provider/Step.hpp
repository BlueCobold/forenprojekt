#pragma once

#ifndef STEP_HPP
#define STEP_HPP

#include "MultiProvider.hpp"
#include "ValueProvider.hpp"

#include <cmath>
#include <exception>
#include <memory>

/// Returns provider 2, if provider 1 < threshold, else provider 3
class Step : public MultiProvider
{
public:

    Step(std::vector<std::unique_ptr<ValueProvider>> provider, float threshold = 1) :
       MultiProvider(std::move(provider)),
       m_threshold(threshold)
    {
        if(getProvider().size() != 3)
            throw std::runtime_error(utility::replace(utility::translateKey("ThreeChilds"), "Step"));
    }

    virtual float getValue() override
    {
        if(getProvider()[0]->getValue() < m_threshold)
            return getProvider()[1]->getValue();
        else
            return getProvider()[2]->getValue();
    }

    virtual std::unique_ptr<ValueProvider> clone() const override
    {
        auto list = cloneProviders();
        return std::unique_ptr<Step>(new Step(std::move(list)));
    }

private:
    float m_threshold;
};

#endif //STEP_HPP
