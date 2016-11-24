#pragma once

#ifndef GESTURE_FILTER_HPP
#define GESTURE_FILTER_HPP

#include <algorithm>
#include <cmath>
#include <vector>

struct FilterResult
{
    FilterResult(bool isValid, float angle) : 
        isValid(isValid),
        angle(angle)
    { }

    bool isValid;
    float angle;
};

class GestureFilter
{
public:
    GestureFilter(float distanceThreshold) :
        m_distanceThreshold(distanceThreshold),
        m_distanceThresholdSquare(distanceThreshold * distanceThreshold),
        m_lastX(0),
        m_lastY(0)
    { }

    void start(float x, float y)
    {
        m_lastX = x;
        m_lastY = y;
    }

    FilterResult filter(float x, float y)
    {
        float xDiff = x - m_lastX;
        float yDiff = y - m_lastY;
        m_lastX = x;
        m_lastY = y;
        if(xDiff * xDiff + yDiff * yDiff < m_distanceThresholdSquare)
            return FilterResult(false, 0);

        return FilterResult(true, atan2f(xDiff, yDiff));
    }

private:
    float m_distanceThreshold;
    float m_distanceThresholdSquare;
    float m_lastX;
    float m_lastY;
};

class GesturePart
{
public:
    GesturePart(float minAngle, float maxAngle) :
        m_overflow(minAngle > maxAngle),
        m_minAngle(minAngle),
        m_maxAngle(maxAngle)
    { }

    bool isInRange(float angle)
    {
        if(m_overflow)
            return !(angle > m_maxAngle && angle < m_minAngle);
        else
            return angle >= m_minAngle && angle <= m_maxAngle;
    }

private:
    bool m_overflow;
    float m_minAngle;
    float m_maxAngle;
};

#include <iostream>
class Gesture
{
public:
    Gesture(float filterThreshold, std::vector<GesturePart> parts) :
        m_filter(filterThreshold),
        m_parts(parts),
        m_currentPart(-1)
    { }

    void start(float x, float y)
    {
        m_filter.start(x, y);
        m_currentPart = -1;
    }

    void process(float x, float y)
    {
        if(m_currentPart + 1 == m_parts.size())
            return;

        auto result = m_filter.filter(x, y);
        if(!result.isValid)
            return;
        
        if(m_currentPart == -1)
        {
            if(m_parts[0].isInRange(result.angle))
            {
                m_currentPart = 0;
                std::cerr<<"0"<<m_currentPart<<std::endl;
            }
            return;
        }

        if(!m_parts[m_currentPart].isInRange(result.angle))
        {
            if(m_currentPart + 1 < static_cast<int>(m_parts.size()) && m_parts[m_currentPart + 1].isInRange(result.angle))
            {
                m_currentPart++;
                std::cerr<<"+"<<m_currentPart<<std::endl;
            }
            else
            {
                m_currentPart = -1;
                std::cerr<<"-"<<std::endl;
            }
        }
    }

    bool detected()
    {
        return m_currentPart + 1 == m_parts.size();
    }

private:
    GestureFilter m_filter;
    std::vector<GesturePart> m_parts;
    int m_currentPart;
};

#endif
