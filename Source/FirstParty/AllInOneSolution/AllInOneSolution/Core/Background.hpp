#pragma once

#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "AnimatedGraphics.hpp"

/// This class will be used to bind an image which will be rendered as background
class Background : public AnimatedGraphics
{
private:

    Animation* m_updatingAni;

public:

	Background();
	~Background();

    void update(const float time);

    virtual float getValueOf(const std::string& name) const;
};

#endif	//BACKGROUND_HPP

