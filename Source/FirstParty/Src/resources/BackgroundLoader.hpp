#pragma once

#ifndef BACKGROUNDLOADER_HPP
#define BACKGROUNDLOADER_HPP

#include <SFML/System/Thread.hpp>
#include <memory>

/// run a function of a class in separate thread
template<class T>
class BackgroundLoader
{
private:
    bool m_loaded;
    bool m_loadInProgress;

    std::unique_ptr<sf::Thread> m_backgroundProcess;

    void(T::*m_function)();
    T& m_object;

    void load()
    {
        m_loadInProgress = true;
        (m_object.*m_function)();
        m_loadInProgress = false;
        m_loaded = true;
    }

public:
    BackgroundLoader(void(T::*function)(), T& object) :
        m_loaded(false),
        m_loadInProgress(false),
        m_function(function),
        m_backgroundProcess(nullptr),
        m_object(object)
    {
        m_backgroundProcess = std::unique_ptr<sf::Thread>(new sf::Thread(&BackgroundLoader::load, this));
    }

    void run()
    {
        if(!m_loaded && !m_loadInProgress)
            m_backgroundProcess->launch();
    }

    void stop()
    {
        m_backgroundProcess->terminate();
        m_loadInProgress = false;
    }

    void reset()
    {
        m_backgroundProcess->terminate();
        m_loadInProgress = false;
        m_loaded = false;
    }

    bool isLoaded()
    {
        return m_loaded;
    }

    bool isLoading()
    {
        return m_loadInProgress;
    }
};

#endif