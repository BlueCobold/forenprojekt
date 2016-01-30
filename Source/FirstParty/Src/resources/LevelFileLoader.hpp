#pragma once

#ifndef LEVEL_FILE_LOADER_HPP
#define LEVEL_FILE_LOADER_HPP

#include "../animation/Animation.hpp"
#include "../animation/CloneHandler.hpp"
#include "../animation/ParticleTrail.hpp"
#include "../animation/provider/ValueProvider.hpp"
#include "../model/SoundObject.hpp"
#include "../model/AnimatedGraphics.hpp"
#include "../model/Entity.hpp"

#include "ResourceManager.hpp"
#include "ProviderParser.hpp"

#include <memory> // unique_ptr, move
#include <string>
#include <unordered_map>
#include <vector>

#include <tinyxml2.h>

class LevelFileLoader
{
public:

    LevelFileLoader(ProviderContext context,
                    ResourceManager& resourceManager) :
        m_context(context),
        m_providerParser(context),
        m_resourceManager(resourceManager)
    { }

    static std::vector<std::string> parseGrid(const tinyxml2::XMLElement& xml);

    std::unique_ptr<ParticleTrail> parseTrail(const tinyxml2::XMLElement& xml);

private:

    ProviderContext m_context;
    ProviderParser m_providerParser;
    ResourceManager& m_resourceManager;
};

#endif // LEVEL_FILE_LOADER_HPP
