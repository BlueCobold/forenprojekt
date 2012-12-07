#pragma once

#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "ResourceCache.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <string>
#include <unordered_map>
#include <utility> // pair

namespace tinyxml2
{
    class XMLDocument;
}

class ResourceManager
{
public:

    ResourceManager();

    sf::Texture* getTexture(const std::string& key);
    sf::Font* getFont(const std::string& key);

private:

    void parseTextures(tinyxml2::XMLDocument& doc);
    void parseFonts(tinyxml2::XMLDocument& doc);


    static sf::Texture* loadTexture(const std::string& path, bool smooth)
    {
        sf::Texture* texture = new sf::Texture;
        if(!texture->loadFromFile("res/img/" + path))
        {
            delete texture; // No memory leak
            return nullptr;
        }
        else
        {
            texture->setSmooth(smooth);
            return texture;
        }
    }

    static sf::Font* loadFont(const std::string& path)
    {
        sf::Font* font = new sf::Font;
        if(!font->loadFromFile("res/font/" + path))
            return nullptr;
        else
            return font;
    }

private:
    std::unordered_map<std::string, std::pair<std::string, bool>> m_textureKeys;
    std::unordered_map<std::string, std::string> m_fontKeys;

    ResourceCache<sf::Texture> m_textures;
    ResourceCache<sf::Font> m_fonts;

};

#endif // RESOURCE_MANAGER_HPP
