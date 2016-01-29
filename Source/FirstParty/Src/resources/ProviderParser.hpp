#pragma once

#ifndef PROVIDER_PARSER_HPP
#define PROVIDER_PARSER_HPP

#include "../animation/provider/ValueProvider.hpp"
#include "ProviderParserContext.hpp"

#include <memory>
#include <vector>

#include "tinyxml2.h"

class ProviderParser
{
    ProviderParserContext m_context;

public:
    ProviderParser(ProviderParserContext context) :
        m_context(context)
    { }

    std::vector<std::unique_ptr<ValueProvider>> parseMultiple(const tinyxml2::XMLElement& xml);

    std::unique_ptr<ValueProvider> parseSingle(const tinyxml2::XMLElement& xml);

private:
    std::vector<float> parseFloatList(const std::string& valueString);
};

#endif // PROVIDER_PARSER_HPP
