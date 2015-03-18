#pragma once

#ifndef CLONEABLE_HPP
#define CLONEABLE_HPP

class Cloneable
{
public:
    virtual Cloneable* clone() const = 0;
};

#endif // CLONEABLE_HPP
