#pragma once

#ifndef ANIMATION_PARSER_HPP
#define ANIMATION_PARSER_HPP

#include "../animation/Animation.hpp"
#include "AnimationContext.hpp"
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
    AnimationContext m_context;
    ProviderParser m_providerParser;
    Callback m_callback;

public:
    AnimationParser(const AnimationContext& context) :
        m_context(context),
        m_providerParser(context.providerContext)
    { }

    std::vector<std::unique_ptr<Animation>> parseMultiple(const tinyxml2::XMLElement& xml) const;
    
    std::vector<std::unique_ptr<Animation>> parseSingle(const tinyxml2::XMLElement& xml) const;
    
    std::unique_ptr<Animation> parseSingleTag(const tinyxml2::XMLElement& xml) const;

    AnimationParser& withElementCallback(Callback callback)
    {
        m_callback = callback;
        return *this;
    }
};

#endif // ANIMATION_PARSER_HPP
