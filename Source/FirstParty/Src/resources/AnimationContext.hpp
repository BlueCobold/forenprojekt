#pragma once

#ifndef ANIMATION_CONTEXT_HPP
#define ANIMATION_CONTEXT_HPP

#include "ProviderContext.hpp"

class ResourceManager;

class AnimationContext
{
public:
    ProviderContext providerContext;
    unsigned int defaultTargetBuffer;
    ResourceManager& resourceManager;

    AnimationContext(const ProviderContext& providerContext,
                     ResourceManager& resourceManager,
                     unsigned int defaultTargetBuffer) :
        providerContext(providerContext),
        resourceManager(resourceManager),
        defaultTargetBuffer(defaultTargetBuffer)
    { }
};

#endif // ANIMATION_CONTEXT_HPP
