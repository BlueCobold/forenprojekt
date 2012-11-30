#pragma once

#ifndef ANIMATED_GRAPHICS_HPP
#define ANIMATED_GRAPHICS_HPP

#include "GraphicalObject.hpp"
#include "animation/OrientedObject.hpp"
#include "animation/TimedObject.hpp"

/// This class defines typical behavior of graphical objects
class AnimatedGraphics : public GraphicalObject, public TimedObject, public OrientedObject
{
};

#endif // ANIMATED_GRAPHICS_HPP
