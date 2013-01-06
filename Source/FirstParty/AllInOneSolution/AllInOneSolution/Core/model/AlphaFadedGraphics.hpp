#pragma once

#ifndef ALPHAFADED_GRAPHICS_HPP
#define ALPHAFADED_GRAPHICS_HPP

#include "AnimatedGraphics.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>

class AlphaFadedGraphics : public GraphicalObject, public TimedObject
{
public:

	AlphaFadedGraphics(const sf::Texture& texture, const float startAlpha, const float targetAlpha, const float duration);
	~AlphaFadedGraphics();

	void update();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	bool isFinished() const { return _isFinished; }

private:
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	float m_startAlpha;
	float m_alphaDelta;
	float m_duration;
	float m_currentAlpha;
	sf::Clock m_frameTime;
	bool _isFinished;
};

#endif // ANIMATED_GRAPHICS_HPP
