#pragma once

#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "ResourceCache.hpp"
#include "../gui/BitmapFont.hpp"
#include "../gui/MenuTemplate.hpp"
#include "SoundBufferManager.hpp"
#include "SpriteSheet.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/OpenGL.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <utility> // pair

namespace tinyxml2
{
    class XMLDocument;
}

class ResourceManager : public SoundBufferManager
{
public:

    ResourceManager();

    sf::Texture* getTexture(const std::string& key);
    sf::Font* getFont(const std::string& key);
    sf::SoundBuffer* getSoundBuffer(const std::string& key) override;
    BitmapFont* getBitmapFont(const std::string& key);
    MenuTemplate* getMenuTemplate(const std::string& name);
    SoundManager& getSoundManager();
    SpriteSheet* getSpriteSheet(const std::string& name);

private:

    void parseTextures(tinyxml2::XMLDocument& doc);
    void parseFonts(tinyxml2::XMLDocument& doc);
    void parseSounds(tinyxml2::XMLDocument& doc);
    void parseBitmapFonts(tinyxml2::XMLDocument& doc);
    void parseMenus(tinyxml2::XMLDocument& doc);
    void parseSpriteSheet(tinyxml2::XMLDocument& doc);

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
            glFlush();
            return texture;
        }
    }

    static sf::Font* loadFont(const std::string& path)
    {
        sf::Font* font = new sf::Font;
        if(!font->loadFromFile("res/font/" + path))
        {
            delete font;
            return nullptr;
        }
        else
            return font;
    }

    static sf::SoundBuffer* loadSoundBuffer(const std::string& path)
    {
        sf::SoundBuffer* soundBuffer = new sf::SoundBuffer;
        if(!soundBuffer->loadFromFile("res/audio/" + path))
        {
            delete soundBuffer;
            return nullptr;
        }
        else
            return soundBuffer;
    }

    BitmapFont* loadBitmapFont(const std::string& path)
    {
        BitmapFont* font = new BitmapFont;
        if(!font->loadFromFile("res/bitmapfont/" + path, *this))
        {
            delete font;
            return nullptr;
        }
        else
            return font;
    }

    static SpriteSheet* loadSpriteSheet(const std::string& path)
    {
        SpriteSheet* sheet = new SpriteSheet;
        if(!sheet->loadFromFile("res/spritesheet/" + path))
        {
            delete sheet;
            return nullptr;
        }
        else
            return sheet;
    }

private:

    std::unordered_map<std::string, std::pair<std::string, bool>> m_textureKeys;
    std::unordered_map<std::string, std::string> m_fontKeys;
    std::unordered_map<std::string, std::string> m_soundBufferKeys;
    std::unordered_map<std::string, std::string> m_bitmapFontKeys;
    std::unordered_map<std::string, std::string> m_menuKeys;
    std::unordered_map<std::string, std::string> m_spriteSheetKeys;

    std::unique_ptr<SoundManager> m_soundManager;
    ResourceCache<sf::Texture> m_textures;
    ResourceCache<sf::Font> m_fonts;
    ResourceCache<sf::SoundBuffer> m_soundBuffers;
    ResourceCache<BitmapFont> m_bitmapFonts;
    ResourceCache<MenuTemplate> m_menus;
    ResourceCache<SpriteSheet> m_spriteSheets;
};

#endif // RESOURCE_MANAGER_HPP
