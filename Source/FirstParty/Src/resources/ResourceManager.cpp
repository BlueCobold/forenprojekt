#include "ResourceManager.hpp"
#include "MenuLoader.hpp"
#include "ShaderLoader.hpp"
#include "SpriteSheet.hpp"
#include "../MacHelper.hpp"
#include "../gui/BitmapFont.hpp"
#include "../rendering/GLExt.hpp"

#include <tinyxml2.h>

#include <exception>
#include <functional> // bind

ResourceManager::ResourceManager(ShaderContext& context) : 
    m_context(&context)
{
    m_soundManager = std::unique_ptr<SoundManager>(new SoundManager(*this));
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
    parseFonts(doc);
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
    m_soundManager->clear();
}

sf::Texture* ResourceManager::loadTexture(const std::string& path, bool smooth)
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
        gl::Flush();
        return texture;
    }
}

sf::Font* ResourceManager::loadFont(const std::string& path)
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
sf::SoundBuffer* ResourceManager::loadSoundBuffer(const std::string& path)
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

BitmapFont* ResourceManager::loadBitmapFont(const std::string& path)
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

SpriteSheet* ResourceManager::loadSpriteSheet(const std::string& path)
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

CryptoPP::RSA::PublicKey* ResourceManager::loadPublicKey(const std::string& path)
{
    CryptoPP::RSA::PublicKey* publicKey = new CryptoPP::RSA::PublicKey;
    CryptoPP::ByteQueue queue;
    CryptoPP::FileSource file((resourcePath() + "res/key/" + path).c_str(), true);

    file.TransferTo(queue);
    queue.MessageEnd();
    publicKey->Load(queue);
    return publicKey;
}

sf::Music* ResourceManager::loadMusic(const std::string& path)
{
    sf::Music* music = new sf::Music;
    if(!music->openFromFile(resourcePath() + "res/music/" + path))
    {
        delete music;
        return nullptr;
    }
    else
        return music;
}

const BitmapFont* ResourceManager::getBitmapFont(const std::string& key)
{
    auto self = this;
    return getOrFail<const BitmapFont, std::string>(m_bitmapFontKeys, m_bitmapFonts, key,
        [=](const std::string& path)->std::function<BitmapFont*()>
        {
            return [=](){ return self->loadBitmapFont(path); };
        }, "UnknownBitmapFont");
}

const MenuTemplate* ResourceManager::getMenuTemplate(const std::string& key)
{
    auto self = this;
    return getOrFail<MenuTemplate, std::string>(m_menuKeys, m_menus, key,
        [=](const std::string& path)->std::function<MenuTemplate*()>
        {
            return [=](){ return MenuLoader::loadMenuTemplate(std::string("res/menus/") + path, *self); };
        }, "UnknownMenuTemplate");
}

#ifndef NO_SOUND
sf::SoundBuffer* ResourceManager::getSoundBuffer(const std::string& key)
{
    return getOrFail<sf::SoundBuffer, std::string>(m_soundBufferKeys, m_soundBuffers, key,
        [=](const std::string& path)->std::function<sf::SoundBuffer*()>
        {
            return [=](){ return ResourceManager::loadSoundBuffer(path); };
        }, "UnknownSound");
}
#endif

const sf::Texture* ResourceManager::getTexture(const std::string& key)
{
    return getOrFail<const sf::Texture, TextureParams>(m_textureKeys, m_textures, key,
        [=](const TextureParams& params)->std::function<const sf::Texture*()>
        {
            return [=](){ return ResourceManager::loadTexture(std::get<0>(params), std::get<1>(params)); };
        }, "UnknownTexture");
}

void ResourceManager::addTexture(const std::string& key, const sf::Texture& texture)
{
    m_textures.put(key, texture);
}

Shader* ResourceManager::getShader(const std::string& key)
{
    if(!Shader::isAvailable())
        throw std::runtime_error(utility::translateKey("ShadersNotAvailable"));

    auto self = this;
    return getOrFail<Shader, ShaderParams>(m_shaderKeys, m_shaders, key,
        [=](const ShaderParams& params)->std::function<Shader*()>
        {
            return [=](){ return ShaderLoader::loadShader(std::get<0>(params), std::get<1>(params), std::get<2>(params), *self); };
        }, "UnknownShader");
}

ShaderContext& ResourceManager::getShaderContext() const
{
    return *m_context;
}

const SpriteSheet* ResourceManager::getSpriteSheet(const std::string& key)
{
    return getOrFail<const SpriteSheet, std::string>(m_spriteSheetKeys, m_spriteSheets, key,
        [=](const std::string& path)->std::function<SpriteSheet*()>
        {
            return [=](){ return ResourceManager::loadSpriteSheet(path); };
        }, "UnknownSpriteSheet");
}

const sf::Font* ResourceManager::getFont(const std::string& key)
{
    return getOrFail<const sf::Font, std::string>(m_fontKeys, m_fonts, key,
        [=](const std::string& path)->std::function<sf::Font*()>
        {
            return [=](){ return ResourceManager::loadFont(path); };
        }, "UnknownFont");
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

void ResourceManager::parseFonts(tinyxml2::XMLDocument& doc)
{
    parse(doc, "fonts", "font", [&](const tinyxml2::XMLElement* element)
    {
        m_fontKeys.insert(std::make_pair(std::string(element->Attribute("name")),
                                         std::string(element->Attribute("path"))));
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
    return *m_soundManager;
}

const std::unordered_map<int, std::string>& ResourceManager::getFileNames()
{
    return m_levelFileNames;
}

void ResourceManager::parsePublicKeys(tinyxml2::XMLDocument& doc)
{
    if(auto keys = doc.FirstChildElement("Keys"))
    {
        for(auto it = keys->FirstChildElement("Key");
            it != nullptr; it = it->NextSiblingElement("Key"))
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
        if(m_publicKeys.exists(name))
            return m_publicKeys.get(name);
        else
        {
            if(m_publicKeys.load(name, [path, this](){ return loadPublicKey(path); }))
                return m_publicKeys.get(name);
        }
    }

    throw std::runtime_error(utility::replace(utility::translateKey("UnknownPublicKey"), key));
}

void ResourceManager::parseHashValues(tinyxml2::XMLDocument& doc)
{
    if(auto hashValues = doc.FirstChildElement("HashValues"))
    {
        for(auto it = hashValues->FirstChildElement("HashValue");
            it != nullptr; it = it->NextSiblingElement("HashValue"))
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

    throw std::runtime_error(utility::replace(utility::translateKey("UnknownHashValue"), key));
}

void ResourceManager::parseMusic(tinyxml2::XMLDocument& doc)
{
    parse(doc, "songs", "song", [&](const tinyxml2::XMLElement* element)
    {
        m_musicKeys.insert(std::make_pair(std::string(element->Attribute("name")),
                                         std::string(element->Attribute("path"))));
    });
}

sf::Music* ResourceManager::getMusic(const std::string& key)
{
    return getOrFail<sf::Music, std::string>(m_musicKeys, m_music, key,
        [=](const std::string& path)->std::function<sf::Music*()>
        {
            return [=](){ return ResourceManager::loadMusic(path); };
        }, "UnknownMusic");
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