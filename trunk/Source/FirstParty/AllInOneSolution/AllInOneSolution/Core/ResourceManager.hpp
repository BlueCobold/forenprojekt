#pragma once

#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "ResourceCache.hpp"

#include <SFML/Graphics/Texture.hpp>

#include <string>

class ResourceManager
{
public:
    sf::Texture* getTexture(const std::string& key);

private:
    static sf::Texture* loadTexture(const std::string& key)
    {
        sf::Texture* tex = new sf::Texture;
        if(!tex->loadFromFile("res/img/" + key))
            return nullptr;
        else
            return tex;
    }

private:
    ResourceCache<sf::Texture> m_textures;
};

#endif // RESOURCE_MANAGER_HPP
