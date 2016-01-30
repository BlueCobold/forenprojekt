#pragma once

#ifndef ANIMATION_PARSER_HPP
#define ANIMATION_PARSER_HPP

#include "../animation/Animation.hpp"
#include "ProviderContext.hpp"
#include "ProviderParser.hpp"

#include <memory>
#include <vector>

#include "tinyxml2.h"

class ResourceManager;

class AnimationParser
{
    ProviderContext m_context;
    ProviderParser m_providerParser;
    ResourceManager& m_resourceManager;

public:
    AnimationParser(ProviderContext context, ResourceManager& resourceManager) :
        m_context(context),
        m_providerParser(context),
        m_resourceManager(resourceManager)
    { }

    std::vector<std::unique_ptr<Animation>> parseMultiple(const tinyxml2::XMLElement& xml);

    std::unique_ptr<Animation> parseSingle(const tinyxml2::XMLElement& xml);
};

#endif // ANIMATION_PARSER_HPP
