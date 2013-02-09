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
    parseBitmapFonts(doc);
}

BitmapFont* ResourceManager::getBitmapFont(const std::string& key)
{
    auto bitmapFont = m_bitmapFontKeys.find(key);
    
    if(bitmapFont != m_bitmapFontKeys.end() && bitmapFont->first == key)
    {
        if(m_bitmapFonts.exists(bitmapFont->first))
            return m_bitmapFonts.get(bitmapFont->first);
        else
        {
            if(m_bitmapFonts.load(bitmapFont->first,
                std::bind(&ResourceManager::loadBitmapFont, bitmapFont->second, this)))
                return m_bitmapFonts.get(bitmapFont->first);
        }
    }

    return nullptr;
}

sf::SoundBuffer* ResourceManager::getSoundBuffer(const std::string& key)
{
    // Does the key even exist?
    auto sound = m_soundBufferKeys.find(key);

    if(sound != m_soundBufferKeys.end() && sound->first == key)
    {
        // Sound already loaded
        if(m_soundBuffers.exists(sound->first))
            return m_soundBuffers.get(sound->first);
        else
        {
            if(m_soundBuffers.load(sound->first,
                std::bind(&ResourceManager::loadSoundBuffer, sound->second)))
                return m_soundBuffers.get(sound->first);    
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
        m_soundBufferKeys.insert(std::make_pair<std::string, std::string>(
            std::string(soundIterator->Attribute("name")), std::string(soundIterator->Attribute("path"))));
    }
}

void ResourceManager::parseBitmapFonts(tinyxml2::XMLDocument& doc)
{
    tinyxml2::XMLElement* bitmapFonts = doc.FirstChildElement("bitmapfonts");

    for(auto it = bitmapFonts->FirstChildElement("bitmapfont");
        it != nullptr; it = it->NextSiblingElement("bitmapfont"))
    {
        m_bitmapFontKeys.insert(std::make_pair<std::string, std::string>(
            std::string(it->Attribute("name")), std::string(it->Attribute("path"))));
    }
}