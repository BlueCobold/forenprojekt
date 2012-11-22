#include "ResourceManager.hpp"

sf::Texture* ResourceManager::getTexture(const std::string& key)
{
    if(m_textures.exists(key))
    {
        return m_textures.get(key);
    }
    else
    {
        if(m_textures.loadFromKey(key, &ResourceManager::loadTexture))
            return m_textures.get(key);
        else
            return nullptr;
    }
}

sf::Font* ResourceManager::getFont(const std::string& key)
{
    if(m_fonts.exists(key))
    {
        return m_fonts.get(key);
    }
    else
    {
        if(m_fonts.loadFromKey(key, &ResourceManager::loadFont))
            return m_fonts.get(key);
        else
            return nullptr;
    }
}
