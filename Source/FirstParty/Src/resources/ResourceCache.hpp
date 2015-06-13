#pragma once

#ifndef RESOURCE_CACHE_HPP
#define RESOURCE_CACHE_HPP

#include <exception>
#include <functional> // function
#include <map>
#include <memory> // unique_ptr
#include <string>
#include <utility> // make_pair
#include "../Utility.hpp"

#include <SFML/Audio/SoundBuffer.hpp>

/// This class is a light header-only ResourceManager
/// and is capabale of handling all sorts of resources,
/// including the ones from SFML.
/// Author & Maintainer: ftb
template <typename T>
class ResourceCache
{
public:

    typedef std::string Key;
    typedef std::unique_ptr<T> Value;
    typedef std::function<T*()> Functor;
    typedef std::function<T*(const Key&)> Functor2;
    typedef std::function<void(T*)> Callback;

private:

    typedef std::map<Key, Value> Map;

public:
    
    ResourceCache() : m_externalCallback(nullptr)
    { }

    bool load(const Key& key, const Functor& func)
    {
        auto it = m_resources.find(key);
        if(it != end(m_resources))
            return true;

        auto resource = func();
        if(resource == nullptr)
            throw std::runtime_error(utility::replace(utility::translateKey("LoadFail"), key));

        m_resources.insert(std::make_pair(key, std::unique_ptr<T>(resource)));
        return true;
    }

    // Overloading doesn't work... reasonable name?
    bool loadFromKey(const Key& key, const Functor2& func)
    {
        auto it = m_resources.find(key);
        if(it == end(m_resources))
            m_resources.insert(std::make_pair(key, std::unique_ptr<T>(func(key))));

        return true;
    }

    bool exists(const Key& key)
    {
        return (m_resources.find(key) != end(m_resources)) || (m_externalResources.find(key) != end(m_externalResources));
    }
    
    /// Add an external resource to the pool. This resource will not be handled by this manager, but
    /// still will be returned to everyone asking for it by its name.
    void put(const Key& key, T& resource)
    {
        m_externalResources.insert(std::make_pair(key, &resource));
    }

    T* get(const Key& key)
    {
        auto it = m_resources.find(key);
        if(it != end(m_resources))
            return it->second.get();
        auto ext = m_externalResources.find(key);
        if(ext != end(m_externalResources))
        {
            if(m_externalCallback)
                m_externalCallback(ext->second);
            return ext->second;
        }

        return nullptr;
    }

    void registerExternalResourceRequested(Callback func)
    {
        m_externalCallback = func;
    }

private:

    std::map<Key, Value> m_resources;
    std::map<Key, T*> m_externalResources;
    Callback m_externalCallback;
};

#endif // RESOURCE_CACHE_HPP
