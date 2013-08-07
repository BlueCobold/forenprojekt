#pragma once

#ifndef RANDOM_PROVIDER_HPP
#define RANDOM_PROVIDER_HPP

#include "ValueProvider.hpp"

#include <random>

/// Returns a random real number in [min, max)
class RandomProvider : public ValueProvider
{
private:

    const std::uniform_real_distribution<float> m_distribution;
    std::default_random_engine m_random_engine;

public:

    RandomProvider(const float min, const float max) : m_distribution(min, max)
    {
	    m_random_engine.seed(std::random_device()());
    }

    virtual float getValue() override
    {
	    return m_distribution(m_random_engine);
    }
};

#endif //RANDOM_PROVIDER_HPP