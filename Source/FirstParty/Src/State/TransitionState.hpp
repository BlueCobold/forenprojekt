#pragma once

#ifndef TRANSITIONSTATE_HPP
#define TRANSITIONSTATE_HPP

#include "State.hpp"
#include "../rendering/transitions/Transition.hpp"
#include "../resources/ResourceManager.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

#include <memory>
#include <vector>

/// This state is responsible for showing a transition.
class TransitionState : public State
{
public:

    TransitionState(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config, std::vector<sf::RenderTexture*>& m_offscreenBuffers);
    ~TransitionState();

    virtual StateChangeInformation update(const float time);
    virtual void draw(const DrawParameter& params);
    virtual void onEnter(const EnterStateInformation* enterInformation, const float time);

private:
    void render(const EnterTransitionStateInformation* info,
                sf::RenderTexture& target,
                State* state,
                const float time);

    std::unique_ptr<Transition> m_transition;
    StateId m_followingState;
    EnterStateInformation* m_followingEnterInformation;
    
    std::vector<sf::RenderTexture*> m_offscreenBuffers;
    sf::RenderTexture m_sourceImage;
    sf::RenderTexture m_targetImage;
};

#endif // TRANSITIONSTATE_HPP