#pragma once

#ifndef RANDOMTRANSITION_HPP
#define RANDOMTRANSITION_HPP

#include "Transition.hpp"

#include <memory>

namespace sf
{
    class Texture;
}

class RandomTransition : public Transition
{
public:
    enum TransitionType {HorizontalHalves = 0,     /* needs only source*/
      /*Last Source ->*/ VerticalHalves,           /* needs only source*/
      /*neutral     ->*/ Alpha,                    /* no source or target needed */
                         GrowingCircle,            /* needs only target*/
                         GrowingRectangle,         /* needs only target*/
                         HorizontalMaskingStripes, /* needs only target*/
                         HorizontalSlices,         /* needs only target*/
                         HorizontalStripes,        /* needs only target*/
                         RotatingSquares,          /* needs only target*/
                         VerticalMaskingStripes,   /* needs only target*/
                         VerticalSlices,           /* needs only target*/
      /*Last Target ->*/ VerticalStripes,          /* needs only target*/
                         VerticalSlidingStripes,   /* needs both texture*/
                         HorizontalSlidingStripes, /* needs both texture*/
                         HorizontalCompress,       /* needs both texture*/
                         VerticalCompress,         /* needs both texture*/
                         HorizontalExpand,         /* needs both texture*/
                         VerticalExpand,           /* needs both texture*/
                         HorizontalSpring,         /* needs both texture*/
                         VerticalSpring,           /* needs both texture*/
                         /* TypeCount must always be the last entry*/
     /*Type Counter ->*/ TypeCount};
private:
    int m_maxHorizontalLines;
    int m_maxVerticalLines;

    static int m_lastTransition;
    std::unique_ptr<Transition> m_transition;

    int randomNumber(bool sourceExist, bool targetExist);

    std::unique_ptr<Transition> createTransition(int randomCount);

public:

    RandomTransition(const sf::Texture* sourceTexture,
                     const sf::Texture* targetTexture,
                     const float duration,
                     const sf::Vector2u& size);

    virtual void update();

    virtual void draw(const DrawParameter& param);
};

#endif // RANDOMTRANSITION_HPP