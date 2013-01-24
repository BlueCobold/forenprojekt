#include "ResourceManager.hpp"

#include <tinyxml2.h>

#include <functional> // bind

ResourceManager::ResourceManager()
{
    // Parse resource information
    tinyxml2::XMLDocument doc;
    doc.LoadFile("res/resources.nfo");

    if(doc.Error()) // Error while loading file
    {
        doc.PrintError();
        return;
    }

    parseSounds(doc);
    parseTextures(doc);
    parseFonts(doc);
}

sf::Sound* ResourceManager::getSound(const std::string& key)
{
    // Does the key even exist?
    auto sound = m_soundKeys.find(key);

    if(sound != m_soundKeys.end() && sound->first == key)
    {
        // Sound already loaded
        if(m_sounds.exists(sound->first))
            return m_sounds.get(sound->first);
        else
        {
            if(m_sounds.load(sound->first,
                std::bind(&ResourceManager::loadSound, sound->second, &m_soundBuffer)))
                return m_sounds.get(sound->first);    
        }
    }
    
    // If the key doesn't exist
    return nullptr;
}

sf::Texture* ResourceManager::getTexture(const std::string& key)
{
    // Does the key even exist?
    auto texture = m_textureKeys.find(key);

    if(texture != m_textureKeys.end() && texture->first == key)
    {
        // Texture already loaded
        if(m_textures.exists(texture->second.first))
            return m_textures.get(texture->second.first);
        else
        {
            if(m_textures.load(texture->second.first,
                std::bind(&ResourceManager::loadTexture, texture->second.first, texture->second.second)))
                return m_textures.get(texture->second.first);
        }
    }
    
    // If the key doesn't exist
    return nullptr;
}

sf::Font* ResourceManager::getFont(const std::string& key)
{
    // Does the key even exist?
    auto font = m_fontKeys.find(key);

    if(font != m_fontKeys.end() && font->first == key)
    {
        // Texture already loaded
        if(m_fonts.exists(font->second))
            return m_fonts.get(font->second);
        else
        {
            if(m_fonts.load(font->second,
                std::bind(&ResourceManager::loadFont, font->second)))
                return m_fonts.get(font->second);
        }
    }
    
    // If the key doesn't exist
    return nullptr;
}

void ResourceManager::parseTextures(tinyxml2::XMLDocument& doc)
{
    tinyxml2::XMLElement* textures = doc.FirstChildElement("textures");

    for(auto textureIterator = textures->FirstChildElement("texture");
        textureIterator != nullptr; textureIterator = textureIterator->NextSiblingElement("texture"))
    {
        auto properties = std::make_pair<std::string, bool>(
            std::string(textureIterator->Attribute("path")),
            textureIterator->BoolAttribute("smooth"));

        m_textureKeys.insert(std::make_pair<std::string, std::pair<std::string, bool>>(
            std::string(textureIterator->Attribute("name")), properties));
    }
}

void ResourceManager::parseFonts(tinyxml2::XMLDocument& doc)
{
    tinyxml2::XMLElement* fonts = doc.FirstChildElement("fonts");

    for(auto fontIterator = fonts->FirstChildElement("font");
        fontIterator != nullptr; fontIterator = fontIterator->NextSiblingElement("font"))
    {
        m_fontKeys.insert(std::make_pair<std::string, std::string>(
            std::string(fontIterator->Attribute("name")), std::string(fontIterator->Attribute("path"))));
    }
}

void ResourceManager::parseSounds(tinyxml2::XMLDocument& doc)
{
    tinyxml2::XMLElement* sounds = doc.FirstChildElement("sounds");

    for(auto soundIterator = sounds->FirstChildElement("sound");
        soundIterator != nullptr; soundIterator = soundIterator->NextSiblingElement("sound"))
    {
        m_soundKeys.insert(std::make_pair<std::string, std::string>(
            std::string(soundIterator->Attribute("name")), std::string(soundIterator->Attribute("path"))));
    }
}