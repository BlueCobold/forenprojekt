#pragma once

#ifndef TRANSITIONSTATE_HPP
#define TRANSITIONSTATE_HPP

#include "State.hpp"
#include "rendering/transitions/Transition.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

#include <memory>

/// This state is responsible for showing a transition.
class TransitionState : public State
{
public:

    TransitionState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config);
    ~TransitionState();

    virtual StateChangeInformation update(const float time);
    virtual void draw(const DrawParameter& params);
    virtual void onEnter(const EnterStateInformation* enterInformation, const float time);

private:

    std::unique_ptr<Transition> m_transition;
    StateId m_followingState;
    EnterStateInformation* m_followingEnterInformation;

    sf::RenderTexture m_sourceImage;
    sf::RenderTexture m_targetImage;
};

#endif // TRANSITIONSTATE_HPP