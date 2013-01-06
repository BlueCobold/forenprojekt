
#include "AlphaFadedGraphics.hpp"

AlphaFadedGraphics::AlphaFadedGraphics(const sf::Texture& texture, const float startAlpha, const float targetAlpha, const float duration):
	m_startAlpha(startAlpha),
	m_alphaDelta(std::abs(startAlpha - targetAlpha)),
	m_duration(duration),
	m_currentAlpha(startAlpha),
	m_texture(texture),
	_isFinished(false)
{
	TimedObject::restartAt(0);
	if (startAlpha > targetAlpha) m_alphaDelta = -m_alphaDelta;

	m_sprite.setTexture(m_texture, true);
	m_sprite.setPosition(0,0);
}

AlphaFadedGraphics::~AlphaFadedGraphics()
{
}

void AlphaFadedGraphics::update()
{
	TimedObject::updateCurrentTime(m_frameTime.getElapsedTime().asSeconds());

	float elapsed = TimedObject::getPassedTime();
	if (elapsed > m_duration)
	{
		elapsed = m_duration;
		_isFinished = true;
	}

	m_currentAlpha = m_startAlpha + m_alphaDelta * elapsed / m_duration;

	m_sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * m_currentAlpha)));
}

void AlphaFadedGraphics::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	GraphicalObject::draw(target, states);
	target.draw(m_sprite, states);
}