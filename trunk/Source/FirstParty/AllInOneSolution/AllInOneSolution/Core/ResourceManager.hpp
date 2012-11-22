#pragma once

#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "ResourceCache.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <string>

class ResourceManager
{
public:

    sf::Texture* getTexture(const std::string& key);
    sf::Font* getFont(const std::string& key);

private:

    static sf::Texture* loadTexture(const std::string& key)
    {
        sf::Texture* texture = new sf::Texture;
        if(!texture->loadFromFile("res/img/" + key))
            return nullptr;
        else
            return texture;
    }

    static sf::Font* loadFont(const std::string& key)
    {
        sf::Font* font = new sf::Font;
        if(!font->loadFromFile("res/font/" + key))
            return nullptr;
        else
            return font;
    }

private:

    ResourceCache<sf::Texture> m_textures;
    ResourceCache<sf::Font> m_fonts;

};

#endif // RESOURCE_MANAGER_HPP
