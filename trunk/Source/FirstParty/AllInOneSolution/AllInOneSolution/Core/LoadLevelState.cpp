#include "LoadLevelState.hpp"
#include "resources/ResourceManager.hpp"
#include "rendering/transitions/GrowingRectangleTransition.hpp"
#include "rendering/transitions/GrowingCircleTransition.hpp"
#include "rendering/transitions/HorizontalMaskingStripesTransition.hpp"
#include "rendering/transitions/VerticalMaskingStripesTransition.hpp"
#include "rendering/transitions/VerticalStripesTransition.hpp"
#include "rendering/transitions/HorizontalStripesTransition.hpp"
#include "rendering/transitions/AlphaTransition.hpp"
#include "rendering/transitions/VerticalSlidingStripesTransition.hpp"
#include "rendering/transitions/HorizontalSlidingStripesTransition.hpp"
#include "rendering/transitions/HorizontalHalvesTransition.hpp"
#include "rendering/transitions/VerticalHalvesTransition.hpp"
#include "rendering/transitions/HorizontalSlicesTransition.hpp"
#include "rendering/transitions/VerticalSlicesTransition.hpp"
#include "rendering/transitions/RotatingSquaresTransition.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "gui/HUD.hpp"

#include <iostream>

LoadLevelState::LoadLevelState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config) :
    State(screen, resourceManager, config),
    m_level(nullptr),
    m_fadedScreen(nullptr)
{
    m_level = new Level(3, m_resourceManager, m_config);
    bool fromImage = m_texture.create(screen.getSize().x, screen.getSize().y);

    float time = m_frametime.getElapsedTime().asSeconds();
    m_level->restartAt(time);
    m_level->update(time, m_texture);

    HUD hud(resourceManager, config);

    hud.update(m_level, m_frametime.getElapsedTime().asSeconds());

    m_texture.clear();
    m_level->draw(DrawParameter(m_texture));
    hud.draw(m_texture);
    m_texture.display();

    m_fadedScreen = std::unique_ptr<Transition>(new RotatingSquaresTransition(nullptr, &m_texture.getTexture(), 10,10, 0.5f));
}

LoadLevelState::~LoadLevelState()
{

}

void LoadLevelState::onEnter(void* enterInformation)
{
}

StateChangeInformation LoadLevelState::update()
{
    m_fadedScreen->update();
    if(m_fadedScreen->isFinished())
        return StateChangeInformation(PlayStateId, m_level);

    return StateChangeInformation::Empty();
}

void LoadLevelState::draw()
{
    m_screen.clear();
    m_fadedScreen->draw(DrawParameter(m_screen));
}