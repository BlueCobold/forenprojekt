#pragma once

#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "ResourceCache.hpp"
#include "../gui/BitmapFont.hpp"
#include "../gui/MenuTemplate.hpp"
#include "SoundBufferManager.hpp"
#include "SpriteSheet.hpp"
#include "PathHelper.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/OpenGL.hpp>

#include <rsa.h>
#include <osrng.h>
#include <files.h>

#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>

namespace tinyxml2
{
    class XMLDocument;
}

class ResourceManager : public SoundBufferManager
{
public:

    ResourceManager();

    const sf::Texture* getTexture(const std::string& key);
    
    sf::Shader* getShader(const std::string& key);

    const sf::Font* getFont(const std::string& key);
#ifndef NO_SOUND
    sf::SoundBuffer* getSoundBuffer(const std::string& key) override;
#endif
    const BitmapFont* getBitmapFont(const std::string& key);
    const MenuTemplate* getMenuTemplate(const std::string& name);
    SoundManager& getSoundManager();
    const SpriteSheet* getSpriteSheet(const std::string& name);
    const std::unordered_map<int, std::string>& getFileNames();
    CryptoPP::RSA::PublicKey* ResourceManager::getPublicKey(const std::string& key);
    std::string getHashValue(const std::string& key);

private:

    void parseTextures(tinyxml2::XMLDocument& doc);
    void parseShaders(tinyxml2::XMLDocument& doc);
    void parseFonts(tinyxml2::XMLDocument& doc);
    void parseSounds(tinyxml2::XMLDocument& doc);
    void parseBitmapFonts(tinyxml2::XMLDocument& doc);
    void parseMenus(tinyxml2::XMLDocument& doc);
    void parseSpriteSheet(tinyxml2::XMLDocument& doc);
    void parseLevelFileName(tinyxml2::XMLDocument& doc);
    void parsePublicKeys(tinyxml2::XMLDocument& doc);
    void parseHashValues(tinyxml2::XMLDocument& doc);

    static void parse(const tinyxml2::XMLDocument& doc,
                      const std::string& parent,
                      const std::string& element,
                      std::function<void(const tinyxml2::XMLElement*)> operation);

    static sf::Texture* loadTexture(const std::string& path, bool smooth)
    {
        sf::Texture* texture = new sf::Texture;
        if(!texture->loadFromFile(resourcePath() + "res/img/" + path))
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

    static sf::Shader* loadShader(const std::string& vertexPath, const std::string& fragmentPath)
    {
        if(!sf::Shader::isAvailable())
            return nullptr;

        sf::Shader* shader = new sf::Shader;
        if(!shader->loadFromFile(resourcePath() + "res/shader/" + vertexPath, resourcePath() + "res/shader/" + fragmentPath))
        {
            delete shader;
            return nullptr;
        }
        else
        {
            glFlush();
            return shader;
        }
    }

    static sf::Font* loadFont(const std::string& path)
    {
        sf::Font* font = new sf::Font;
        if(!font->loadFromFile(resourcePath() + "res/font/" + path))
        {
            delete font;
            return nullptr;
        }
        else
            return font;
    }
    
#ifndef NO_SOUND
    static sf::SoundBuffer* loadSoundBuffer(const std::string& path)
    {
        sf::SoundBuffer* soundBuffer = new sf::SoundBuffer;
        if(!soundBuffer->loadFromFile(resourcePath() + "res/audio/" + path))
        {
            delete soundBuffer;
            return nullptr;
        }
        else
            return soundBuffer;
    }
#endif
    
    BitmapFont* loadBitmapFont(const std::string& path)
    {
        BitmapFont* font = new BitmapFont;
        if(!font->loadFromFile(resourcePath() + "res/bitmapfont/" + path, *this))
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
        if(!sheet->loadFromFile(resourcePath() + "res/spritesheet/" + path))
        {
            delete sheet;
            return nullptr;
        }
        else
            return sheet;
    }

    static CryptoPP::RSA::PublicKey* loadPublicKey(const std::string& path)
    {
        CryptoPP::RSA::PublicKey* publicKey = new CryptoPP::RSA::PublicKey;
        CryptoPP::ByteQueue queue;
        CryptoPP::FileSource file((resourcePath() + "res/key/" + path).c_str(), true);

        file.TransferTo(queue);
        queue.MessageEnd();
        publicKey->Load(queue);
        return publicKey;
    }

private:

    typedef std::tuple<std::string, std::string> ShaderParams;
    typedef std::tuple<std::string, bool> TextureParams;

    std::unordered_map<std::string, TextureParams> m_textureKeys;
    std::unordered_map<std::string, ShaderParams> m_shaderKeys;
    std::unordered_map<std::string, std::string> m_fontKeys;
    std::unordered_map<std::string, std::string> m_soundBufferKeys;
    std::unordered_map<std::string, std::string> m_bitmapFontKeys;
    std::unordered_map<std::string, std::string> m_menuKeys;
    std::unordered_map<std::string, std::string> m_spriteSheetKeys;
    std::unordered_map<int, std::string> m_levelFileNames;
    std::unordered_map<std::string, std::string> m_publicKeyKeys;
    std::unordered_map<std::string, std::string> m_hashValues;

    std::unique_ptr<SoundManager> m_soundManager;
    ResourceCache<const sf::Texture> m_textures;
    ResourceCache<sf::Shader> m_shaders;
    ResourceCache<const sf::Font> m_fonts;
#ifndef NO_SOUND
    ResourceCache<sf::SoundBuffer> m_soundBuffers;
#endif
    ResourceCache<const BitmapFont> m_bitmapFonts;
    ResourceCache<MenuTemplate> m_menus;
    ResourceCache<const SpriteSheet> m_spriteSheets;
    
    template<typename C, typename T>
    C* getOrFail(std::unordered_map<std::string, T>& container,
                 ResourceCache<C>& cache,
                 const std::string& key,
                 std::function<std::function<C*()>(const T&)> operation,
                 const std::string& errorKey)
    {
        auto result = cache.get(key);
        if(result)
            return result;

        auto entry = container.find(key);
        if(entry != end(container))
        {
            if(cache.load(key, operation(entry->second)))
                 return cache.get(key);
        }

        throw std::runtime_error(utility::replace(utility::translateKey(errorKey), key));
    }
    ResourceCache<CryptoPP::RSA::PublicKey> m_publicKeys;
};

#endif // RESOURCE_MANAGER_HPP
