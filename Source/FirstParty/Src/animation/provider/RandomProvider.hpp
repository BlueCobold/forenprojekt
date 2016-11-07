#pragma once

#ifndef RANDOM_PROVIDER_HPP
#define RANDOM_PROVIDER_HPP

#include "ValueProvider.hpp"

#include <random>

/// Returns a random real number in [min, max)
class RandomProvider : public ValueProvider
{
    double m_min, m_max;

    static bool _inited;
    static std::random_device _device;
    static std::uniform_real_distribution<double> _distribution;
    static std::default_random_engine _random_engine;

    static void init()
    {
        if(!_inited)
        {
            _random_engine.seed(_device());
            _inited = true;
        }
    }

    std::unique_ptr<ValueProvider> doClone() const override
    {
        return std::unique_ptr<RandomProvider>(new RandomProvider(m_min, m_max));
    }

    double calculateValue() override
    {
        return m_min + (m_max - m_min) * _distribution(_random_engine);
    }

public:
    RandomProvider(const double min, const double max) :
        m_min(min), m_max(max)
    {
        init();
    }
};

#endif //RANDOM_PROVIDER_HPP
