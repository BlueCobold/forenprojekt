#pragma once

#ifndef GESTURE_HANDLER_HPP
#define GESTURE_HANDLER_HPP

#include "GestureFilter.hpp"
#include "../resources/AppConfig.hpp"
#include "../Input.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <cmath>
#include <functional>
#include <utility>
#include <vector>

class GestureHandler
{
public:
    GestureHandler(const sf::RenderWindow& screen) :
        m_screen(screen)
    { }

    void addGesture(Gesture gesture, std::function<void()> action)
    {
        m_gestures.push_back(std::make_pair(gesture, action));
    }

    void process()
    {
#if defined(IOS) || defined(ANDROID)
        auto mPos = m_screen.mapPixelToCoords(utility::Mouse.getTouchPosition());
#else
        auto mPos = m_screen.mapPixelToCoords(sf::Mouse::getPosition(m_screen));
#endif
        if(utility::Mouse.leftButtonDown())
        {
            for(auto it = begin(m_gestures); it != end(m_gestures); ++it)
                it->first.start(mPos.x, mPos.y);
        }
        else if(utility::Mouse.leftButtonPressed())
        {
            for(auto it = begin(m_gestures); it != end(m_gestures); ++it)
                it->first.process(mPos.x, mPos.y);
        }
        else if(utility::Mouse.leftButtonReleased())
        {
            for(auto it = begin(m_gestures); it != end(m_gestures); ++it)
                if(it->first.detected())
                    it->second();
        }
    }

private:
    const sf::RenderWindow& m_screen;
    std::vector<std::pair<Gesture, std::function<void()>>> m_gestures;
};

#endif
