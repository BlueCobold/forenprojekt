#pragma once

#ifndef RANDOMTRANSITION_HPP
#define RANDOMTRANSITION_HPP

#include <vector>
#include "Transition.hpp"

namespace sf
{
    class Texture;
}

class RandomTransition
{
private:
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
                         /* TypeCount must allways be the last entry*/
     /*Type Counter ->*/ TypeCount};

    int m_maxHorizontalLines;

    int m_maxVerticalLines;

    int m_lastTransition;

    std::vector<std::unique_ptr<Transition>> m_transition;

    int randomNumber(bool sourceExist, bool targetExist);

    Transition* createTransition(const sf::Texture* sourceTexture,
                                 const sf::Texture* targetTexture,
                                 const float duration,
                                 int randomCount);
public:
    Transition* getRandomTransition(const sf::Texture* sourceTexture,
                                    const sf::Texture* targetTexture,
                                    const float duration);

    RandomTransition(int maxHorizontalLines, int maxVerticalLines);
};

#endif // RANDOMTRANSITION_HPP