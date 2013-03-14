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

private:

	typedef std::map<Key, Value> Map;

public:
	
	bool load(const Key& key, const Functor& func)
	{
		auto it = m_resources.find(key);

		if(it == end(m_resources))
		{
            T* resource = func();
            if(resource == nullptr)
                throw std::exception(utility::replace(utility::translateKey("LoadFail"), key).c_str());
			m_resources.insert(std::make_pair(key, std::unique_ptr<T>(resource)));
		}
		return true;
	}

	// Overloding doesn't work... reasonable name?
	bool loadFromKey(const Key& key, const Functor2& func)
	{
		auto it = m_resources.find(key);

		if(it == end(m_resources))
		{
			m_resources.insert(std::make_pair(key, std::unique_ptr<T>(func(key))));
		}
		return true;
	}

    bool exists(const Key& key)
    {
        return (m_resources.find(key) != end(m_resources));
    }

	// return the resource
	T* get(const Key& key)
	{
		auto it = m_resources.find(key);

		if(it != end(m_resources))
		{
			return (it->second).get();
		}
		return nullptr;
	}

private:

	std::map<Key, Value> m_resources;

};

#endif // RESOURCE_CACHE_HPP
