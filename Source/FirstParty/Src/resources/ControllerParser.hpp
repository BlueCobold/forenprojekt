#pragma once

#ifndef CONTROLLER_PARSER_HPP
#define CONTROLLER_PARSER_HPP

#include "../animation/provider/ValueProvider.hpp"
#include "../model/Array.hpp"
#include "ProviderContext.hpp"
#include "ProviderParser.hpp"

#include <memory>
#include <array>

#include "tinyxml2.h"

class ResourceManager;


class ControllerParser
{
    ProviderContext m_context;
    ProviderParser m_providerParser;

public:
    ControllerParser(ProviderContext context) :
        m_context(context),
        m_providerParser(context)
    { }

    Array<std::unique_ptr<ValueProvider>, 4> parseColor(const tinyxml2::XMLElement& xml);

    Array<std::unique_ptr<ValueProvider>, 2> parsePosition(const tinyxml2::XMLElement& xml);

    Array<std::unique_ptr<ValueProvider>, 2> parseScale(const tinyxml2::XMLElement& xml);

    std::unique_ptr<ValueProvider> parseRotation(const tinyxml2::XMLElement& xml);

    std::unique_ptr<ValueProvider> findController(
        const tinyxml2::XMLElement& xml,
        const std::string& childName,
        const std::string& propertyName,
        const std::string& propertyValue);
};

#endif // CONTROLLER_PARSER_HPP
