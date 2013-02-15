#pragma once

#ifndef ANIMATED_GRAPHICS_HPP
#define ANIMATED_GRAPHICS_HPP

#include "../rendering/GraphicalObject.hpp"
#include "../animation/AnimatedObject.hpp"
#include "../animation/VariableHolder.hpp"

/// This class defines typical behavior of graphical objects
class AnimatedGraphics : public GraphicalObject, public virtual AnimatedObject
{
};

#endif // ANIMATED_GRAPHICS_HPP
