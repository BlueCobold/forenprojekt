#pragma once

#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include "DrawParameter.hpp"

class Drawable
{
public:
    virtual void draw(const DrawParameter& params) = 0;
};

#endif // DRAWABLE_HPP
