#pragma once

#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "ResourceCache.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

//#include <memory>
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
    sf::Sound* getSound(const std::string& key);

private:

    void parseTextures(tinyxml2::XMLDocument& doc);
    void parseFonts(tinyxml2::XMLDocument& doc);
    void parseSounds(tinyxml2::XMLDocument& doc);


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

    static sf::Sound* loadSound(const std::string& path, sf::SoundBuffer* soundBuffer)
    {
        sf::Sound* sound = new sf::Sound;
        sound->setVolume(100.f);
        sound->setBuffer(*soundBuffer);
        if(!soundBuffer->loadFromFile("res/audio/" + path))
            return nullptr;
        else
            return sound;
    }

private:
    sf::SoundBuffer m_soundBuffer;

    std::unordered_map<std::string, std::pair<std::string, bool>> m_textureKeys;
    std::unordered_map<std::string, std::string> m_fontKeys;
    std::unordered_map<std::string, std::string> m_soundKeys;

    ResourceCache<sf::Texture> m_textures;
    ResourceCache<sf::Font> m_fonts;
    ResourceCache<sf::Sound> m_sounds;

};

#endif // RESOURCE_MANAGER_HPP
