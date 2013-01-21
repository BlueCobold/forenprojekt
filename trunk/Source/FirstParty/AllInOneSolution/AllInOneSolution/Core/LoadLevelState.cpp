#include "LoadLevelState.hpp"
#include "resources/ResourceManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

LoadLevelState::LoadLevelState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config) :
    State(screen, resourceManager, config),
    m_level(nullptr),
	m_alphaFadedScreen(nullptr)
{
    screen.setFramerateLimit(60);
	m_level = new Level(2, m_resourceManager, m_config);
    m_level->restartAt(m_frametime.getElapsedTime().asSeconds());
    m_level->update(m_frametime.getElapsedTime().asSeconds(), screen);
	m_level->draw(screen);

	sf::Image img = screen.capture();
	sf::Texture texture;
	bool fromImage = texture.loadFromImage(screen.capture());

	m_alphaFadedScreen = std::unique_ptr<AlphaFadedGraphics>(new AlphaFadedGraphics(texture, 0.0f, 1.0f, 0.5f));

	screen.setView(screen.getDefaultView());
}

LoadLevelState::~LoadLevelState()
{

}

void LoadLevelState::onEnter(void* enterInformation)
{
}

StateChangeInformation LoadLevelState::update()
{
	m_alphaFadedScreen->update();
	if (m_alphaFadedScreen->isFinished()) return StateChangeInformation(PlayStateId, m_level);

	return StateChangeInformation::Empty();
}

void LoadLevelState::draw()
{
	m_screen.clear(sf::Color(0,0,0,255));
	m_screen.draw(*m_alphaFadedScreen.get());
}