#pragma once

#ifndef ANIMATED_GRAPHICS_HPP
#define ANIMATED_GRAPHICS_HPP

#include "../rendering/GraphicalObject.hpp"
#include "../animation/AnimatedObject.hpp"

/// This class defines typical behavior of graphical objects
class AnimatedGraphics : public GraphicalObject, public AnimatedObject
{
};

#endif // ANIMATED_GRAPHICS_HPP
