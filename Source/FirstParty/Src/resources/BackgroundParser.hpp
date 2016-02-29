#pragma once

#ifndef BACKGROUND_PARSER_HPP
#define BACKGROUND_PARSER_HPP

#include "AnimationParser.hpp"
#include "ProviderParser.hpp"
#include "TemplateParser.hpp"
#include "../model/Background.hpp"

#include <SFML/System/Vector2.hpp>

#include <memory>
#include <vector>

#include <tinyxml2.h>

class ResourceManager;

class BackgroundParser
{
private:
    const ProviderContext& m_context;
    ResourceManager& m_resourceManager;

public:
    BackgroundParser(const ProviderContext& context, ResourceManager& resourceManager) :
        m_context(context),
        m_resourceManager(resourceManager)
    { }

    std::unique_ptr<Background> parse(
        const tinyxml2::XMLElement& xml,
        const sf::Vector2u& size,
        unsigned int defaultTargetBuffer) const;
};

#endif // BACKGROUND_PARSER_HPP
