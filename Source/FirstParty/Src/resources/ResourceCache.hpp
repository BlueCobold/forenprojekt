#pragma once

#ifndef RESOURCE_CACHE_HPP
#define RESOURCE_CACHE_HPP

#include <exception>
#include <functional>
#include <deque>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include "../Utility.hpp"

#include <SFML/System/Err.hpp>
/// This class is a light header-only cache
/// and is capabale of handling all sorts of resources,
/// including the ones from SFML.
template <typename T>
class ResourceCache
{
public:
    ResourceCache() :
        m_parent(nullptr)
    { }

    typedef std::string Key;
    typedef std::unique_ptr<T> Resource;
    typedef std::function<std::unique_ptr<T>()> Functor;

    T* load(const Key& key, const Functor& func)
    {
       auto it = get(key);
        if(it != nullptr)
            return it;

        auto resource = func();
        if(resource == nullptr)
            throw std::runtime_error(utility::replace(utility::translateKey("@LoadFail"), key));

        auto ptr = resource.get();
        m_resources.insert(std::make_pair(key, std::move(resource)));
        return ptr;
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
        return get(key, nullptr);
    }

    ResourceCache<T>* addScope(const Key& scope)
    {
        m_scopes.emplace(std::make_pair(scope, std::deque<std::unique_ptr<ResourceCache<T>>>()));
        m_scopes[scope].push_front(std::unique_ptr<ResourceCache<T>>(new ResourceCache<T>(this)));
        return m_scopes[scope].front().get();
    }

    void purge(const Key& scope)
    {
        auto scopeIt = m_scopes.find(scope);
        if(scopeIt == end(m_scopes))
            return;

        auto first = std::move(scopeIt->second.front());
        scopeIt->second.clear();
        scopeIt->second.push_front(std::move(first));
    }

private:
    ResourceCache<T>* m_parent;
    std::map<Key, Resource> m_resources;
    std::map<Key, T*> m_externalResources;
    std::map<Key, std::deque<std::unique_ptr<ResourceCache<T>>>> m_scopes;
    
    ResourceCache(ResourceCache<T>* parent) :
        m_parent(parent)
    { }

    T* get(const Key& key, ResourceCache<T>* origin, bool keepInScope = false)
    {
        if(this == origin)
            return nullptr;

        auto it = m_resources.find(key);
        if(it != end(m_resources))
            return it->second.get();

        auto ext = m_externalResources.find(key);
        if(ext != end(m_externalResources))
            return ext->second;

        auto scoped = getFromScopes(key, keepInScope);
        if(scoped.first)
        {
            auto res = scoped.first.get();
            m_resources[key] = std::move(scoped.first);
            return res;
        }
        if(scoped.second)
            return scoped.second;

        // if the resource wasn't found anywhere in nested scopes, try parent scopes
        if(m_parent != nullptr)
            return m_parent->get(key, this, true);

        return nullptr;
    }

    std::pair<Resource, T*> getFromScopes(const std::string& key, bool keepInScope)
    {
        auto it = m_resources.find(key);
        if(it != end(m_resources))
        {
            if(!keepInScope)
            {
                auto resource = std::move(it->second);
                m_resources.erase(it);
                return std::make_pair<Resource, T*>(std::move(resource), nullptr);
            }
            else
                return std::make_pair<Resource, T*>(nullptr, it->second.get());
        }
        // try to find the resource in all scopes and all their cycles
        for(auto scopeIt = begin(m_scopes); scopeIt != end(m_scopes);  ++scopeIt)
        {
            for(auto cycleIt = begin(scopeIt->second); cycleIt != end(scopeIt->second);  ++cycleIt)
            {
                auto resource = (*cycleIt)->getFromScopes(key, false);
                if(resource.second)
                    return std::make_pair<Resource, T*>(nullptr, resource.second);

                // if it shall be kept in scope, move it to first element, otherwise return ownership
                if(resource.first)
                {
                    if(!keepInScope)
                        return std::make_pair<Resource, T*>(std::move(resource.first), nullptr);
                    else
                    {
                        auto ptr = resource.first.get();
                        auto& currentScope = m_scopes[scopeIt->first];
                        auto& firstInScope = currentScope.front();
                        firstInScope->m_resources[key] = std::move(resource.first);
                        return std::make_pair<Resource, T*>(nullptr, ptr);
                    }
                }
            }
        }
        return std::pair<Resource, T*>(nullptr, nullptr);
    }
};

#endif // RESOURCE_CACHE_HPP
