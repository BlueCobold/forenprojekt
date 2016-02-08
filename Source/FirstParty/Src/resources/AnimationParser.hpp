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
public:
    typedef std::function<void(std::unique_ptr<Animation>&, const tinyxml2::XMLElement&)> Callback;

private:
    ProviderContext m_context;
    ProviderParser m_providerParser;
    ResourceManager& m_resourceManager;
    unsigned int m_defaultTargetBuffer;
    Callback m_callback;

public:
    AnimationParser(ProviderContext context, ResourceManager& resourceManager, unsigned int defaultTargetBuffer) :
        m_context(context),
        m_providerParser(context),
        m_resourceManager(resourceManager),
        m_defaultTargetBuffer(defaultTargetBuffer)
    { }

    std::vector<std::unique_ptr<Animation>> parseMultiple(const tinyxml2::XMLElement& xml);
    
    std::vector<std::unique_ptr<Animation>> parseSingle(const tinyxml2::XMLElement& xml);
    
    std::unique_ptr<Animation> parseSingleTag(const tinyxml2::XMLElement& xml);

    AnimationParser& withElementCallback(Callback callback)
    {
        m_callback = callback;
        return *this;
    }
};

#endif // ANIMATION_PARSER_HPP
