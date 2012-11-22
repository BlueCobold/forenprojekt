#pragma once

#include <functional> // function
#include <map>
#include <memory> // unique_ptr
#include <string>
#include <utility> // make_pair

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

		if(it == m_resources.end())
		{
			m_resources.insert(std::make_pair(key, std::unique_ptr<T>(func())));
		}
		return true;
	}

	// Überladung geht hier leider nicht.. sinnvoller Name!?
	bool loadFromKey(const Key& key, const Functor2& func)
	{
		auto it = m_resources.find (key);

		if (it == m_resources.end())
		{
			m_resources.insert(std::make_pair(key, std::unique_ptr<T>(func(key))));
		}
		return true;
	}

    bool exists(const Key& key)
    {
        return (m_resources.find(key) != m_resources.end());
    }

	// return the resource
	T* get(const Key& key)
	{
		auto it = m_resources.find(key);

		if(it != m_resources.end())
		{
			return (it->second).get();
		}
		return nullptr;
	}

private:
	std::map<Key, Value> m_resources;
};