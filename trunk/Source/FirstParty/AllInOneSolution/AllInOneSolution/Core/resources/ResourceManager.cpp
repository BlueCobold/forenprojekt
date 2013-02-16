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
    
    if(bitmapFont != end(m_bitmapFontKeys) && bitmapFont->first == key)
    {
        std::string name = bitmapFont->first;
        std::string path = bitmapFont->second;
        if(m_bitmapFonts.exists(name))
            return m_bitmapFonts.get(name);
        else
        {
            if(m_bitmapFonts.load(name, [path, this](){ return loadBitmapFont(path); }))
                return m_bitmapFonts.get(name);
        }
    }

    return nullptr;
}

sf::SoundBuffer* ResourceManager::getSoundBuffer(const std::string& key)
{
    // Does the key even exist?
    auto sound = m_soundBufferKeys.find(key);

    if(sound != end(m_soundBufferKeys) && sound->first == key)
    {
        std::string name = sound->first;
        std::string path = sound->second;
        // Sound already loaded
        if(m_soundBuffers.exists(name))
            return m_soundBuffers.get(name);
        else
        {
            if(m_soundBuffers.load(name, [path](){ return loadSoundBuffer(path); }))
                return m_soundBuffers.get(name);    
        }
    }
    
    // If the key doesn't exist
    return nullptr;
}

sf::Texture* ResourceManager::getTexture(const std::string& key)
{
    // Does the key even exist?
    auto texture = m_textureKeys.find(key);

    if(texture != end(m_textureKeys) && texture->first == key)
    {
        std::string path = texture->second.first;
        // Texture already loaded
        if(m_textures.exists(path))
            return m_textures.get(path);
        else
        {
            if(m_textures.load(path, [path](){ return loadTexture(path, false); }))
                return m_textures.get(path);
        }
    }
    
    // If the key doesn't exist
    return nullptr;
}

sf::Font* ResourceManager::getFont(const std::string& key)
{
    // Does the key even exist?
    auto font = m_fontKeys.find(key);

    if(font != end(m_fontKeys) && font->first == key)
    {
        std::string path = font->second;
        // Texture already loaded
        if(m_fonts.exists(path))
            return m_fonts.get(path);
        else
        {
            if(m_fonts.load(path, [path](){ return loadFont(path); }))
                return m_fonts.get(path);
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