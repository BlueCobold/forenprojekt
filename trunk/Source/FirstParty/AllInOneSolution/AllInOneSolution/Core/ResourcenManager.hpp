#pragma once

#include <memory>
#include <functional>
#include <map>
#include <string>

#include <iostream>

// ftb war hier :D

template <typename T>
class ResourceManager
{	
public:
	typedef std::string Key;
	typedef std::unique_ptr<T> Value;
	typedef std::function<T*()> Functor;
	typedef std::function<T*(const Key&)> Functor2;
private:
	typedef std::map<Key, Value> Map;
public:

	
	bool loadResource (const Key& key, const Functor& func)
	{
		auto it = m_ResourceMap.find (key);

		if (it == m_ResourceMap.end())
		{
			m_ResourceMap.insert(std::make_pair(key, std::unique_ptr<T>(func())));
		}
		return true;
	}

	// Überladung geht hier leider nicht.. sinnvoller Name!?
	bool loadResourceFromKey (const Key& key, const Functor2& func)
	{
		auto it = m_ResourceMap.find (key);

		if (it == m_ResourceMap.end())
		{
			m_ResourceMap.insert(std::make_pair(key, std::unique_ptr<T>(func(key))));
		}
		return true;
	}

	// return the resource
	T* getResource (const Key& key)
	{
		auto it = m_ResourceMap.find (key);

		if (it != m_ResourceMap.end())
		{
			return (it->second).get();
		}
		return nullptr;
	}

private:
	std::map<Key, Value> m_ResourceMap;

};