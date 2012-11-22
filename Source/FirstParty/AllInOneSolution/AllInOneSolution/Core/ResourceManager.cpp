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
            return nullptr;
        else
            return m_textures.get(key);
    }
}
