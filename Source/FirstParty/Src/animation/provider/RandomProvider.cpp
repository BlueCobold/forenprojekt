
#include "RandomProvider.hpp"

bool RandomProvider::_inited = false;
std::random_device RandomProvider::_device;
std::uniform_real_distribution<double> RandomProvider::_distribution(0.f, 1.f);
std::default_random_engine RandomProvider::_random_engine;
