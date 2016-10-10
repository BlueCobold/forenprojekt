#include "TransitionState.hpp"
#include "EnterStateInformation.hpp"
#include "../Utility.hpp"
#include "../rendering/transitions/RandomTransition.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Err.hpp>

#include <cmath>

TransitionState::TransitionState(sf::RenderWindow& screen,
                                 ResourceManager& resourceManager,
                                 AppConfig& config,
                                 std::vector<sf::RenderTexture*>& offscreenBuffers) :
    State(screen, resourceManager, config),
    m_offscreenBuffers(std::move(offscreenBuffers))
{
    auto desktopMode = sf::VideoMode::getDesktopMode();
    auto size = static_cast<int>(desktopMode.width > desktopMode.height ?
        desktopMode.width : desktopMode.height);
    auto powerOfTwo = static_cast<int>(powf(2.f,
        floorf(logf(static_cast<float>(size))/logf(2.f))));
    if(powerOfTwo < size)
        powerOfTwo *= 2;
    // BUG: the PC might not be able to handle this size!
    // we still need a texture as big as possible to handle screen-size-changes during the gameplay
    m_sourceImage.create(powerOfTwo, powerOfTwo, true);
    m_targetImage.create(powerOfTwo, powerOfTwo, true);
}

TransitionState::~TransitionState()
{
}

void TransitionState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    sf::err() << "Entering TransitionState" << std::endl;
    State::onEnter(enterInformation, time);
    sf::err() << "TransitionState: 1" << std::endl;
    const EnterTransitionStateInformation* info = dynamic_cast<const EnterTransitionStateInformation*>(enterInformation);
    sf::err() << "TransitionState: 2" << std::endl;
    m_followingState = info->m_followingState;
    sf::err() << "TransitionState: 3" << std::endl;
    m_followingEnterInformation = info->m_onEnterInformation;
    sf::err() << "TransitionState: 4" << std::endl;

    info->m_source->pause(time);
    sf::err() << "TransitionState: 5" << std::endl;
    render(info, m_sourceImage, info->m_source, time);
    sf::err() << "TransitionState: 6" << std::endl;

    info->m_onEnterInformation->m_prepareOnly = true;
    sf::err() << "TransitionState: 7" << std::endl;
    info->m_target->onEnter(info->m_onEnterInformation, time);
    sf::err() << "TransitionState: 8" << std::endl;
    info->m_target->update(time);
    sf::err() << "TransitionState: 9" << std::endl;
    render(info, m_targetImage, info->m_target, time);
    
    sf::err() << "TransitionState previews rendered" << std::endl;
    m_transition.reset(new RandomTransition(&m_sourceImage.getTexture(), &m_targetImage.getTexture(), 0.5f, m_screen.getSize(), static_cast<RandomTransition::TransitionType>(info->m_transitionType)));
    sf::err() << "Reset random transition" << std::endl;
}

void TransitionState::render(const EnterTransitionStateInformation* info,
            sf::RenderTexture& target,
            State* state,
            const float time)
{
    sf::err() << "TransitionState.render: 1" << std::endl;
    auto size = m_screen.getSize();
    sf::err() << "TransitionState.render: 2" << std::endl;
    target.setView(utility::getDefaultView(target, size));
    sf::err() << "TransitionState.render: 3" << std::endl;
    target.clear();
    sf::err() << "TransitionState.render: 4" << std::endl;
    DrawParameter params(target);
    
    //sf::err() << "TransitionState.render: 5" << std::endl;
    std::unordered_map<const sf::Texture*, sf::RenderTexture*> offscreens;
    for(unsigned int i = 0; i < m_offscreenBuffers.size(); i++)
    {
        params.addTargetBuffer(*m_offscreenBuffers[i]);
        m_offscreenBuffers[i]->clear(sf::Color(0x7f, 0x7f, 0x7f, 0));
        offscreens.insert(std::make_pair(&m_offscreenBuffers[i]->getTexture(), m_offscreenBuffers[i]));
    }
    //sf::err() << "TransitionState.render: 6" << std::endl;

    params.registerBufferRequest([&](const sf::Texture* texture)
    {
        //sf::err() << "TransitionState.buffer request: 1" << std::endl;
        auto pair = offscreens.find(texture);
        if(pair != end(offscreens))
        {
            //sf::err() << "TransitionState.buffer request: 2" << std::endl;
            pair->second->display();
            //sf::err() << "TransitionState.buffer request: 3" << std::endl;
            offscreens.erase(texture);
            //sf::err() << "TransitionState.buffer request: 4" << std::endl;
        }
    });
    
   // sf::err() << "TransitionState.render: 7" << std::endl;
    state->draw(params);
    sf::err() << "TransitionState.render: 8" << std::endl;
    target.display();
    sf::err() << "TransitionState.render: 9" << std::endl;
}

StateChangeInformation TransitionState::update(const float time)
{
    updateTime(time);
    if(m_transition)
    {
        m_transition->update();
        if(m_transition->isFinished())
            return StateChangeInformation(m_followingState, m_followingEnterInformation);
    }

    return StateChangeInformation::Empty();
}

void TransitionState::doDraw(const DrawParameter& params)
{
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), params.getTarget().getSize()));
    m_transition->draw(params);
}
