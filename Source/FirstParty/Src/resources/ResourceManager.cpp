#include "ResourceManager.hpp"
#include "MenuLoader.hpp"
#include "ShaderLoader.hpp"
#include "SpriteSheet.hpp"
#include "../MacHelper.hpp"
#include "../gui/BitmapFont.hpp"
#include "../rendering/GLExt.hpp"
#include "../resources/AppConfig.hpp"
#include <tinyxml2.h>

#include <exception>
#include <functional> // bind

ResourceManager::ResourceManager(ShaderContext& context, AppConfig& config) :
    m_context(context),
    m_config(config),
    m_texturesPtr(&m_textures),
    m_shadersPtr(&m_shaders),
#ifndef NO_SOUND
    m_soundBuffersPtr(&m_soundBuffers),
#endif
    m_bitmapFontsPtr(&m_bitmapFonts),
    m_menusPtr(&m_menus),
    m_spriteSheetsPtr(&m_spriteSheets),
    m_publicKeysPtr(&m_publicKeys),
    m_musicPtr(&m_music)
{
    m_soundManager = std::unique_ptr<SoundManager>(new SoundManager(*this, config));
    m_soundManagerPtr = m_soundManager.get();

    // Parse resource information
    tinyxml2::XMLDocument doc;
    doc.LoadFile((resourcePath() + "res/resources.nfo").c_str());

    if(doc.Error()) // Error while loading file
    {
        doc.PrintError();
        return;
    }

    parseSounds(doc);
    parseTextures(doc);
    parseShaders(doc);
    parseBitmapFonts(doc);
    parseMenus(doc);
    parseSpriteSheet(doc);
    parseLevelFileName(doc);
    parsePublicKeys(doc);
    parseHashValues(doc);
    parseMusic(doc);

    loadAllMusic();
}

ResourceManager::~ResourceManager()
{
    m_soundManagerPtr->clear();
}

std::unique_ptr<ResourceManager> ResourceManager::getSubScope(const std::string& scope)
{
    std::unique_ptr<ResourceManager> sub(new ResourceManager(m_context, m_config));
    sub->m_bitmapFontKeys = m_bitmapFontKeys;
    sub->m_bitmapFontsPtr = m_bitmapFontsPtr->addScope(scope);
    sub->m_hashValues = m_hashValues;
    sub->m_levelFileNames = m_levelFileNames;
    sub->m_menuKeys = m_menuKeys;
    sub->m_menusPtr = m_menusPtr->addScope(scope);
    sub->m_musicKeys = m_musicKeys;
    sub->m_musicPtr = m_musicPtr->addScope(scope);
    sub->m_publicKeyKeys = m_publicKeyKeys;
    sub->m_publicKeysPtr = m_publicKeysPtr->addScope(scope);
    sub->m_shaderKeys = m_shaderKeys;
    sub->m_shadersPtr = m_shadersPtr->addScope(scope);
    sub->m_soundBufferKeys = m_soundBufferKeys;
    sub->m_soundBuffersPtr = m_soundBuffersPtr;
    sub->m_spriteSheetKeys = m_spriteSheetKeys;
    sub->m_spriteSheetsPtr = m_spriteSheetsPtr->addScope(scope);
    sub->m_textureKeys = m_textureKeys;
    sub->m_texturesPtr = m_texturesPtr->addScope(scope);
    return std::move(sub);
}

void ResourceManager::purge(const std::string& scope)
{
    m_bitmapFontsPtr->purge(scope);
    m_menusPtr->purge(scope);
    m_musicPtr->purge(scope);
    m_publicKeysPtr->purge(scope);
    m_shadersPtr->purge(scope);
    m_spriteSheetsPtr->purge(scope);
    m_texturesPtr->purge(scope);
}

std::unique_ptr<sf::Texture> ResourceManager::loadTexture(const std::string& path, bool smooth)
{
    auto texture = std::unique_ptr<sf::Texture>(new sf::Texture);
    auto fullPath = packedPath() + "res/img/" + path;
    if(!texture->loadFromFile(fullPath))
        return nullptr;
    else
    {
        texture->setSmooth(smooth);
        gl::Flush(); /* <<--- Fehler? */
        return texture;
    }
}

#ifndef NO_SOUND
std::unique_ptr<sf::SoundBuffer> ResourceManager::loadSoundBuffer(const std::string& path)
{
    auto soundBuffer = std::unique_ptr<sf::SoundBuffer>(new sf::SoundBuffer);
    if(!soundBuffer->loadFromFile(packedPath() + "res/audio/" + path))
        return nullptr;
    else
        return soundBuffer;
}
#endif

std::unique_ptr<BitmapFont> ResourceManager::loadBitmapFont(const std::string& path)
{
    auto font = std::unique_ptr<BitmapFont>(new BitmapFont);
    if(!font->loadFromFile(resourcePath() + "res/bitmapfont/" + path, *this))
        return nullptr;
    else
        return font;
}

std::unique_ptr<SpriteSheet> ResourceManager::loadSpriteSheet(const std::string& path)
{
    auto sheet = std::unique_ptr<SpriteSheet>(new SpriteSheet);
    if(!sheet->loadFromFile(resourcePath() + "res/spritesheet/" + path))
        return nullptr;
    else
        return sheet;
}

std::unique_ptr<CryptoPP::RSA::PublicKey> ResourceManager::loadPublicKey(const std::string& path)
{
    auto publicKey = std::unique_ptr<CryptoPP::RSA::PublicKey>(new CryptoPP::RSA::PublicKey);
    CryptoPP::ByteQueue queue;
    CryptoPP::FileSource file((resourcePath() + "res/key/" + path).c_str(), true);

    file.TransferTo(queue);
    queue.MessageEnd();
    publicKey->Load(queue);
    return publicKey;
}

std::unique_ptr<sf::Music> ResourceManager::loadMusic(const std::string& path)
{
    auto music = std::unique_ptr<sf::Music>(new sf::Music);
    if(!music->openFromFile(packedPath() + "res/music/" + path))
        return nullptr;
    else
        return music;
}

const BitmapFont* ResourceManager::getBitmapFont(const std::string& key)
{
    auto self = this;
    return getOrFail<const BitmapFont, std::string>(m_bitmapFontKeys, *m_bitmapFontsPtr, key,
        [=](const std::string& path)->std::function<std::unique_ptr<const BitmapFont>()>
        {
            return [=](){ return std::move(self->loadBitmapFont(path)); };
        }, "@UnknownBitmapFont");
}

const MenuTemplate* ResourceManager::getMenuTemplate(const std::string& key)
{
    auto self = this;
    return getOrFail<MenuTemplate, std::string>(m_menuKeys, *m_menusPtr, key,
        [=](const std::string& path)->std::function<std::unique_ptr<MenuTemplate>()>
        {
            return [=]() -> std::unique_ptr<MenuTemplate> {
                MenuLoader loader(*self);
                return loader.loadMenuTemplate(std::string("res/menus/") + path);
            };
        }, "@UnknownMenuTemplate");
}

#ifndef NO_SOUND
sf::SoundBuffer* ResourceManager::getSoundBuffer(const std::string& key)
{
    return getOrFail<sf::SoundBuffer, std::string>(m_soundBufferKeys, *m_soundBuffersPtr, key,
        [=](const std::string& path)->std::function<std::unique_ptr<sf::SoundBuffer>()>
        {
            return [=](){ return ResourceManager::loadSoundBuffer(path); };
        }, "@UnknownSound");
}
#endif

const sf::Texture* ResourceManager::getTexture(const std::string& key)
{
    return getOrFail<const sf::Texture, TextureParams>(m_textureKeys, *m_texturesPtr, key,
        [=](const TextureParams& params)->std::function<std::unique_ptr<const sf::Texture>()>
        {
            return [=](){ return ResourceManager::loadTexture(std::get<0>(params), std::get<1>(params)); };
        }, "@UnknownTexture");
}

void ResourceManager::addTexture(const std::string& key, const sf::Texture& texture)
{
    m_texturesPtr->put(key, texture);
}

Shader* ResourceManager::getShader(const std::string& key)
{
    if(!Shader::isAvailable())
        throw std::runtime_error(utility::translateKey("@ShadersNotAvailable"));

    auto self = this;
    return getOrFail<Shader, ShaderParams>(m_shaderKeys, *m_shadersPtr, key,
        [=](const ShaderParams& params)->std::function<std::unique_ptr<Shader>()>
        {
            return [=](){ return ShaderLoader::loadShader(std::get<0>(params), std::get<1>(params), std::get<2>(params), *self); };
        }, "@UnknownShader");
}

ShaderContext& ResourceManager::getShaderContext() const
{
    return m_context;
}

const SpriteSheet* ResourceManager::getSpriteSheet(const std::string& key)
{
    return getOrFail<const SpriteSheet, std::string>(m_spriteSheetKeys, *m_spriteSheetsPtr, key,
        [=](const std::string& path)->std::function<std::unique_ptr<SpriteSheet>()>
        {
            return [=](){ return ResourceManager::loadSpriteSheet(path); };
        }, "@UnknownSpriteSheet");
}

void ResourceManager::parse(const tinyxml2::XMLDocument& doc,
                            const std::string& parent,
                            const std::string& element,
                            std::function<void(const tinyxml2::XMLElement*)> operation)
{
    if(auto group = doc.FirstChildElement(parent.c_str()))
    {
        for(auto it = group->FirstChildElement(element.c_str());
            it != nullptr; it = it->NextSiblingElement(element.c_str()))
        {
            operation(it);
        }
    }
}

void ResourceManager::parseTextures(tinyxml2::XMLDocument& doc)
{
    parse(doc, "textures", "texture", [&](const tinyxml2::XMLElement* element)
    {
        m_textureKeys.insert(std::make_pair(std::string(element->Attribute("name")),
                                            ResourceManager::TextureParams(
                                                std::string(element->Attribute("path")),
                                                element->BoolAttribute("smooth"))));
    });
}

void ResourceManager::parseShaders(tinyxml2::XMLDocument& doc)
{
    parse(doc, "shaders", "shader", [&](const tinyxml2::XMLElement* element)
    {
        m_shaderKeys.insert(std::make_pair(std::string(element->Attribute("name")),
                                           ResourceManager::ShaderParams(
                                               std::string(element->Attribute("vertexPath")),
                                               std::string(element->Attribute("fragmentPath")),
                                               std::string(element->Attribute("configPath")))));
    });
}

void ResourceManager::parseSounds(tinyxml2::XMLDocument& doc)
{
    parse(doc, "sounds", "sound", [&](const tinyxml2::XMLElement* element)
    {
        m_soundBufferKeys.insert(std::make_pair(std::string(element->Attribute("name")),
                                                std::string(element->Attribute("path"))));
    });
}

void ResourceManager::parseBitmapFonts(tinyxml2::XMLDocument& doc)
{
    parse(doc, "bitmapfonts", "bitmapfont", [&](const tinyxml2::XMLElement* element)
    {
        m_bitmapFontKeys.insert(std::make_pair(std::string(element->Attribute("name")),
                                               std::string(element->Attribute("path"))));
    });
}

void ResourceManager::parseMenus(tinyxml2::XMLDocument& doc)
{
    parse(doc, "menus", "menu", [&](const tinyxml2::XMLElement* element)
    {
        m_menuKeys.insert(std::make_pair(std::string(element->Attribute("name")),
                                         std::string(element->Attribute("path"))));
    });
}

void ResourceManager::parseSpriteSheet(tinyxml2::XMLDocument& doc)
{
    parse(doc, "spritesheets", "spritesheet", [&](const tinyxml2::XMLElement* element)
    {
        m_spriteSheetKeys.insert(std::make_pair(std::string(element->Attribute("name")),
                                                std::string(element->Attribute("path"))));
    });
}

void ResourceManager::parseLevelFileName(tinyxml2::XMLDocument& doc)
{
    parse(doc, "levels", "level", [&](const tinyxml2::XMLElement* element)
    {
        m_levelFileNames.insert(std::make_pair(element->IntAttribute("number"),
                                               std::string(element->Attribute("filename"))));
    });
}

SoundManager& ResourceManager::getSoundManager()
{
    return *m_soundManagerPtr;
}

const std::unordered_map<int, std::string>& ResourceManager::getFileNames()
{
    return m_levelFileNames;
}

void ResourceManager::parsePublicKeys(tinyxml2::XMLDocument& doc)
{
    if(auto keys = doc.FirstChildElement("keys"))
    {
        for(auto it = keys->FirstChildElement("key");
            it != nullptr; it = it->NextSiblingElement("key"))
        {
            m_publicKeyKeys.insert(std::make_pair<std::string, std::string>(
                std::string(it->Attribute("name")), std::string(it->Attribute("path"))));
        }
    }
}

CryptoPP::RSA::PublicKey* ResourceManager::getPublicKey(const std::string& key)
{
    auto publicKey = m_publicKeyKeys.find(key);
    if(publicKey != end(m_publicKeyKeys) && publicKey->first == key)
    {
        std::string name = publicKey->first;
        std::string path = publicKey->second;
        if(m_publicKeysPtr->exists(name))
            return m_publicKeysPtr->get(name);
        else
        {
            if(m_publicKeysPtr->load(name, [path, this](){ return loadPublicKey(path); }))
                return m_publicKeysPtr->get(name);
        }
    }

    throw std::runtime_error(utility::replace(utility::translateKey("@UnknownPublicKey"), key));
}

void ResourceManager::parseHashValues(tinyxml2::XMLDocument& doc)
{
    if(auto hashValues = doc.FirstChildElement("hashvalues"))
    {
        for(auto it = hashValues->FirstChildElement("hashvalue");
            it != nullptr; it = it->NextSiblingElement("hashvalue"))
        {
            m_hashValues.insert(std::make_pair<std::string, std::string>(
                std::string(it->Attribute("filename")), std::string(it->Attribute("value"))));
        }
    }
}

std::string ResourceManager::getHashValue(const std::string& key)
{
    auto result = m_hashValues.find(key);
    if(result != end(m_hashValues))
        return result->second;

    throw std::runtime_error(utility::replace(utility::translateKey("@UnknownHashValue"), key));
}

void ResourceManager::parseMusic(tinyxml2::XMLDocument& doc)
{
    parse(doc, "music", "track", [&](const tinyxml2::XMLElement* element)
    {
        m_musicKeys.insert(std::make_pair(std::string(element->Attribute("name")),
                                         std::string(element->Attribute("filename"))));
    });
}

sf::Music* ResourceManager::getMusic(const std::string& key)
{
    return getOrFail<sf::Music, std::string>(m_musicKeys, *m_musicPtr, key,
        [=](const std::string& path)->std::function<std::unique_ptr<sf::Music>()>
        {
            return [=](){ return ResourceManager::loadMusic(path); };
        }, "@UnknownMusic");
}

void ResourceManager::loadAllMusic()
{
    m_musicList.clear();

    for(auto it = std::begin(m_musicKeys); it != std::end(m_musicKeys); ++it)
    {
        m_musicList.push_back(getMusic(it->first));
    }
}

std::vector<sf::Music*>& ResourceManager::getMusic()
{
    return m_musicList;
}
