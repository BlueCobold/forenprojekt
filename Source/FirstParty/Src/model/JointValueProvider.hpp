#pragma once

#ifndef JOINT_VALUE_PROVIDER_HPP
#define JOINT_VALUE_PROVIDER_HPP

#include "../animation/Animation.hpp"
#include "../animation/provider/Observer.hpp"
#include "../animation/provider/ValueProvider.hpp"
#include "../model/joint/JointObject.hpp"
#include "../Utility.hpp"

#include <functional>

typedef Observer<const JointObject> JointObserver;
typedef Observer<const Animation> AnimationObserver;


enum ProviderLocation : unsigned int
{
    // the position of the joint relative to the body will be returned
    Entity = 0,
    // the position of the anchor relative to the body will be returned
    Anchor = 1,
    // the center between Entity and Anchor will be returned
    Link = 2,
    // the center between body and Entity will be returned
    Intermediate = 3
};

class JointRotationProvider : public ValueProvider, public JointObserver
{
    ProviderLocation m_index;

    std::unique_ptr<ValueProvider> doClone() const override
    {
        return std::unique_ptr<JointRotationProvider>(new JointRotationProvider(getCloneObservable(), getCallback(), m_index));
    }

    float calculateValue() override
    {
        auto points = getObserved().getAnchorPoints();
        auto offsets = getObserved().getAnchorOffsets();
        if(points.size() < 2 || offsets.size() < 1)
            return 0;

        auto& b2start = points.at(0);
        auto& b2end = points.at(1);
        auto diff = b2end - b2start;
        if(m_index == Intermediate)
            diff = offsets[0];

        auto angle = -atan2f(-diff.x, -diff.y);
        auto degree = utility::toDegree(angle);
        if(degree < 0)
            return 360.f + degree;
        return degree;
    }

public:
    JointRotationProvider(const JointObject& joint, const CloneCallback cloneHandler, ProviderLocation index = Link) :
        Observer(joint, cloneHandler),
        m_index(index)
    { }
};

class JointPositionProvider : public ValueProvider, public JointObserver
{
    bool m_useX;
    ProviderLocation m_index;

    std::unique_ptr<ValueProvider> doClone() const override
    {
        return std::unique_ptr<JointPositionProvider>(new JointPositionProvider(getCloneObservable(), m_useX, getCallback(), m_index));
    }

    float calculateValue() override
    {
        auto points = getObserved().getAnchorPoints();
        auto offsets = getObserved().getAnchorOffsets();
        // use no offset -> entity-position
        if(m_index == Entity || points.size() < 2 || offsets.size() < 2)
        {
            if(offsets.size() > 0)
                return utility::toPixel(m_useX ? offsets.at(0).x : offsets.at(0).y);
            return 0;
        }

        auto& entityPos = points.at(0);
        auto& anchorPos = points.at(1);
        auto& entityOff = offsets.at(0);
        //auto& anchorOff = offsets.at(1);
        // need relative position of anchor to entity? calculate diff
        if(m_index == Anchor)
            return utility::toPixel(m_useX ? (anchorPos.x - entityPos.x + entityOff.x) : (anchorPos.y - entityPos.y + entityOff.y));
        else if(m_index == Intermediate)
            return utility::toPixel(m_useX ? entityOff.x/2 : entityOff.y/2);

        return utility::toPixel(m_useX ? ((anchorPos.x - entityPos.x)/2 + entityOff.x) : ((anchorPos.y - entityPos.y)/2 + entityOff.y));
    }

public:
    JointPositionProvider(const JointObject& joint, bool xAxis, const CloneCallback cloneHandler, ProviderLocation index = Link) :
        Observer(joint, cloneHandler),
        m_useX(xAxis),
        m_index(index)
    { }
};

class JointScaleProvider : public ValueProvider, public JointObserver, public AnimationObserver
{
    ProviderLocation m_index;

    std::unique_ptr<ValueProvider> doClone() const override
    {
        return std::unique_ptr<JointScaleProvider>(new JointScaleProvider(JointObserver::getCloneObservable(),
                                                                          AnimationObserver::getCloneObservable(),
                                                                          JointObserver::getCallback(),
                                                                          AnimationObserver::getCallback(),
                                                                          m_index));
    }

    float calculateValue() override
    {
        auto points = JointObserver::getObserved().getAnchorPoints();
        auto offsets = JointObserver::getObserved().getAnchorOffsets();
        if(points.size() < 2 || offsets.size() < 1)
            return 1;

        auto& b2start = points.at(0);
        auto& b2end = points.at(1);
        auto diff = b2end - b2start;
        if(m_index == Intermediate)
            diff = offsets[0];

        auto dist = utility::toPixel(std::sqrtf(diff.x * diff.x + diff.y * diff.y));
        auto& ani = AnimationObserver::getObserved();
        auto size = ani.getSize();
        return dist / size.y;
    }

public:
    JointScaleProvider(const JointObject& joint,
                       const Animation& target,
                       const JointObserver::CloneCallback jointCloneHandler,
                       const AnimationObserver::CloneCallback cloneAnimationHandler,
                       ProviderLocation index = Link) :
        JointObserver(joint, jointCloneHandler),
        AnimationObserver(target, cloneAnimationHandler),
        m_index(index)
    { }
};

#endif //JOINT_VALUE_PROVIDER_HPP
