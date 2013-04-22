#include "RandomTransition.hpp"
#include "GrowingRectangleTransition.hpp"
#include "GrowingCircleTransition.hpp"
#include "HorizontalMaskingStripesTransition.hpp"
#include "VerticalMaskingStripesTransition.hpp"
#include "VerticalStripesTransition.hpp"
#include "HorizontalStripesTransition.hpp"
#include "AlphaTransition.hpp"
#include "VerticalSlidingStripesTransition.hpp"
#include "HorizontalSlidingStripesTransition.hpp"
#include "HorizontalHalvesTransition.hpp"
#include "VerticalHalvesTransition.hpp"
#include "HorizontalSlicesTransition.hpp"
#include "VerticalSlicesTransition.hpp"
#include "RotatingSquaresTransition.hpp"
#include "HorizontalCompressTransition.hpp"
#include "VerticalCompressTransition.hpp"
#include "HorizontalExpandTransition.hpp"
#include "VerticalExpandTransition.hpp"
#include "HorizontalSpringTransition.hpp"
#include "VerticalSpringTransition.hpp"

#include "../../animation/Provider/RandomProvider.hpp"

int RandomTransition::m_lastTransition = static_cast<RandomTransition::TransitionType>(TypeCount);

RandomTransition::RandomTransition(const sf::Texture* sourceTexture,
                                   const sf::Texture* targetTexture,
                                   const float duration,
                                   const sf::Vector2u& size) :
    Transition(sourceTexture, targetTexture, duration, size),
    m_maxHorizontalLines(21),
    m_maxVerticalLines(21),
    m_transition(nullptr)
{
    bool sourceExist = sourceTexture != nullptr;
    bool targetExist = targetTexture != nullptr;
    m_transition = std::move(createTransition(randomNumber(sourceExist, targetExist)));
}

int RandomTransition::randomNumber(bool sourceExist, bool targetExist)
{
    float min = 0;
    float max = static_cast<float>(TypeCount);
    
    if(!sourceExist && !targetExist)
        return static_cast<int>(Alpha);
    else if(!targetExist)
    {
        min = static_cast<float>(HorizontalHalves);
        max = static_cast<float>(Alpha);
    }
    else if(!sourceExist)
    {
        min = static_cast<float>(Alpha);
        max = static_cast<float>(VerticalStripes);
    }
    
    int random = static_cast<int>(RandomProvider(min, max).getValue());
    
    while(random == m_lastTransition)
        random = static_cast<int>(RandomProvider(min, max).getValue());

    m_lastTransition = random;

    return random;
}

std::unique_ptr<Transition> RandomTransition::createTransition(int randomCount)
{

    TransitionType type = static_cast<TransitionType>(randomCount);

    int vertical = static_cast<int>(RandomProvider(2, static_cast<float>(m_maxVerticalLines)).getValue());
    int horizontal = static_cast<int>(RandomProvider(2, static_cast<float>(m_maxHorizontalLines)).getValue());

    auto sourceTexture = getSourceTexture();
    auto targetTexture = getTargetTexture();
    auto duration = getDuration();
    type = GrowingCircle;
    switch(type)
    {
        case HorizontalHalves:
            return std::unique_ptr<Transition>(
                new HorizontalHalvesTransition(sourceTexture, targetTexture, horizontal, duration, getSize()));
        case VerticalHalves:
            return std::unique_ptr<Transition>(
                new VerticalHalvesTransition(sourceTexture, targetTexture, vertical, duration, getSize()));
        case Alpha:
            return std::unique_ptr<Transition>(
                new AlphaTransition(sourceTexture, targetTexture, duration, getSize()));
        case GrowingCircle:
            return std::unique_ptr<Transition>(
                new GrowingCircleTransition(sourceTexture, targetTexture, duration, getSize()));
        case GrowingRectangle:
            return std::unique_ptr<Transition>(
                new GrowingRectangleTransition(sourceTexture, targetTexture, duration, getSize()));
        case HorizontalMaskingStripes:
            return std::unique_ptr<Transition>(
                new HorizontalMaskingStripesTransition(sourceTexture, targetTexture, horizontal, duration, getSize()));
        case HorizontalSlices:
            return std::unique_ptr<Transition>(
                new HorizontalSlicesTransition(sourceTexture, targetTexture, horizontal, duration, getSize()));
        case HorizontalStripes:
            return std::unique_ptr<Transition>(
                new HorizontalStripesTransition(sourceTexture, targetTexture, horizontal, duration, getSize()));
        case RotatingSquares:
            return std::unique_ptr<Transition>(
                new RotatingSquaresTransition(sourceTexture, targetTexture, horizontal, vertical, duration, getSize()));
        case VerticalMaskingStripes:
            return std::unique_ptr<Transition>(
                new VerticalMaskingStripesTransition(sourceTexture, targetTexture, vertical, duration, getSize()));
        case VerticalSlices:
            return std::unique_ptr<Transition>(
                new VerticalSlicesTransition(sourceTexture, targetTexture, vertical, duration, getSize()));
        case VerticalStripes:
            return std::unique_ptr<Transition>(
                new VerticalStripesTransition(sourceTexture, targetTexture, vertical, duration, getSize()));
        case VerticalSlidingStripes:
            return std::unique_ptr<Transition>(
                new VerticalSlidingStripesTransition(sourceTexture, targetTexture, vertical, duration, getSize()));
        case HorizontalSlidingStripes:
            return std::unique_ptr<Transition>(
                new HorizontalSlidingStripesTransition(sourceTexture, targetTexture, horizontal, duration, getSize()));
        case HorizontalCompress:
            return std::unique_ptr<Transition>(
                new HorizontalCompressTransition(sourceTexture, targetTexture, horizontal, duration, getSize()));
        case VerticalCompress:
            return std::unique_ptr<Transition>(
                new VerticalCompressTransition(sourceTexture, targetTexture, vertical, duration, getSize()));
        case HorizontalExpand:
            return std::unique_ptr<Transition>(
                new HorizontalExpandTransition(sourceTexture, targetTexture, horizontal, duration, getSize()));
        case VerticalExpand:
            return std::unique_ptr<Transition>
                (new VerticalExpandTransition(sourceTexture, targetTexture, vertical, duration, getSize()));
        case HorizontalSpring:
            return std::unique_ptr<Transition>(
                new HorizontalSpringTransition(sourceTexture, targetTexture, horizontal - 1, duration, getSize()));
        case VerticalSpring:
            return std::unique_ptr<Transition>(
                new VerticalSpringTransition(sourceTexture, targetTexture, vertical - 1, duration, getSize()));
        case TypeCount:
        default:
        break;
    }
    throw std::runtime_error(utility::replace(utility::translateKey("RandomTransition"), utility::toString(randomCount)));
}

void RandomTransition::update()
{
    Transition::update();
    m_transition->update();
}

void RandomTransition::draw(const DrawParameter& param)
{
    m_transition->draw(param);
}