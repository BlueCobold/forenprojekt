#pragma once

#ifndef PROVIDER_PARSER_HPP
#define PROVIDER_PARSER_HPP

#include "../animation/provider/ValueProvider.hpp"
#include "ProviderContext.hpp"

#include <memory>
#include <vector>

#include "tinyxml2.h"

class ProviderParser
{
    const ProviderContext& m_context;

public:
    ProviderParser(const ProviderContext& context) :
        m_context(context)
    { }

    std::vector<std::unique_ptr<ValueProvider>> parseMultiple(const tinyxml2::XMLElement& xml) const;

    std::unique_ptr<ValueProvider> parseSingle(const tinyxml2::XMLElement& xml) const;

private:
    std::vector<float> parseFloatList(const std::string& valueString) const;
};

#endif // PROVIDER_PARSER_HPP
