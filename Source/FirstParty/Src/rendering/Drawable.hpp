#pragma once

#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include "DrawParameter.hpp"

class Drawable
{
protected:
    virtual void doDraw(const DrawParameter& params) = 0;

public:
    void draw(const DrawParameter& params)
    {
        doDraw(params);
    }
};

#endif // DRAWABLE_HPP
