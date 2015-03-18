#pragma once

#ifndef ANIMATED_OBJECT_HPP
#define ANIMATED_OBJECT_HPP

#include "OrientedObject.hpp"
#include "TimedObject.hpp"

class AnimatedObject : public virtual OrientedObject, public TimedObject, public VariableHandler
{
};

#endif //ANIMATED_OBJECT_HPP
