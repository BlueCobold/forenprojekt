#pragma once

#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "GraphicalObject.hpp"

/// This class will be used to bind an image which will be rendered as background
class Background : public GraphicalObject
{
public:
	Background();
	~Background();

protected:

	void draw(sf::RenderTarget& target, sf::RenderStates states) const; 
};

#endif	//BACKGROUND_HPP

