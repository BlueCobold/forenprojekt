#pragma once

#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "ResourceCache.hpp"
#include "SoundBufferManager.hpp"
#include "PathHelper.hpp"
#include "ShaderContext.hpp"
#include "../animation/AnimatedObject.hpp"
class SpriteSheet;
class BitmapFont;
struct MenuTemplate;
class Shader;
class SoundManager;

#include <tinyxml2.h>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>

#include <rsa.h>
#include <osrng.h>
#include <files.h>

#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>
#include <list>

namespace tinyxml2
{
    class XMLDocument;
}

class AppConfig;

class ResourceManager : public SoundBufferManager
{
public:
    ResourceManager(ShaderContext& context, AppConfig& config);
    ~ResourceManager();

    std::unique_ptr<ResourceManager> getSubScope(const std::string& scope);
    void purge(const std::string& scope);

    const sf::Texture* getTexture(const std::string& key);
    void addTexture(const std::string& key, const sf::Texture& texture);

    Shader* getShader(const std::string& key);
    ShaderContext& getShaderContext() const;

    const sf::Font* getFont(const std::string& key);
#ifndef NO_SOUND
    sf::SoundBuffer* getSoundBuffer(const std::string& key) override;
#endif
    const BitmapFont* getBitmapFont(const std::string& key);
    const MenuTemplate* getMenuTemplate(const std::string& name);
    SoundManager& getSoundManager();
    const SpriteSheet* getSpriteSheet(const std::string& name);
    const std::unordered_map<int, std::string>& getFileNames();
    CryptoPP::RSA::PublicKey* getPublicKey(const std::string& key);
    std::string getHashValue(const std::string& key);
    sf::Music* getMusic(const std::string& key);
    std::vector<sf::Music*>& getMusic();

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
    void parseMusic(tinyxml2::XMLDocument& doc);

    static void parse(const tinyxml2::XMLDocument& doc,
                      const std::string& parent,
                      const std::string& element,
                      std::function<void(const tinyxml2::XMLElement*)> operation);

    static std::unique_ptr<sf::Texture> loadTexture(const std::string& path, bool smooth);
    static std::unique_ptr<sf::Font> loadFont(const std::string& path);
#ifndef NO_SOUND
    static std::unique_ptr<sf::SoundBuffer> loadSoundBuffer(const std::string& path);
#endif
    std::unique_ptr<BitmapFont> loadBitmapFont(const std::string& path);
    static std::unique_ptr<SpriteSheet> loadSpriteSheet(const std::string& path);
    static std::unique_ptr<CryptoPP::RSA::PublicKey> loadPublicKey(const std::string& path);
    static std::unique_ptr<sf::Music> loadMusic(const std::string& path);

private:

    typedef std::tuple<std::string, std::string, std::string> ShaderParams;
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
    std::unordered_map<std::string, std::string> m_musicKeys;

    std::unique_ptr<SoundManager> m_soundManager;
    SoundManager* m_soundManagerPtr;
    ResourceCache<const sf::Texture> m_textures;
    ResourceCache<const sf::Texture>* m_texturesPtr;
    ResourceCache<Shader> m_shaders;
    ResourceCache<Shader>* m_shadersPtr;
    ResourceCache<const sf::Font> m_fonts;
    ResourceCache<const sf::Font>* m_fontsPtr;
#ifndef NO_SOUND
    ResourceCache<sf::SoundBuffer> m_soundBuffers;
    ResourceCache<sf::SoundBuffer>* m_soundBuffersPtr;
#endif
    ResourceCache<const BitmapFont> m_bitmapFonts;
    ResourceCache<const BitmapFont>* m_bitmapFontsPtr;
    ResourceCache<MenuTemplate> m_menus;
    ResourceCache<MenuTemplate>* m_menusPtr;
    ResourceCache<const SpriteSheet> m_spriteSheets;
    ResourceCache<const SpriteSheet>* m_spriteSheetsPtr;
    ResourceCache<CryptoPP::RSA::PublicKey> m_publicKeys;
    ResourceCache<CryptoPP::RSA::PublicKey>* m_publicKeysPtr;
    ResourceCache<sf::Music> m_music;
    ResourceCache<sf::Music>* m_musicPtr;
    ShaderContext& m_context;
    AppConfig& m_config;
    
    std::vector<sf::Music*> m_musicList;

    template<typename C, typename T>
    C* getOrFail(std::unordered_map<std::string, T>& container,
                 ResourceCache<C>& cache,
                 const std::string& key,
                 std::function<std::function<std::unique_ptr<C>()>(const T&)> operation,
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

    void loadAllMusic();
};

#endif // RESOURCE_MANAGER_HPP
