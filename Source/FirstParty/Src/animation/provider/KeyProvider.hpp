#pragma once

#ifndef KEY_PROVIDER_HPP
#define KEY_PROVIDER_HPP

#include <SFML/Window/Keyboard.hpp>

#include "ValueProvider.hpp"
#include "../../Input.hpp"

class KeyProvider : public ValueProvider
{
    sf::Keyboard::Key m_KeyAttachedTo;

    // The array uses the US-ASCII encoding.
    const static sf::Keyboard::Key intToKeys[128];

    std::unique_ptr<ValueProvider> doClone() const override
    {
        for(int i=0; i<128; i++)
            if(m_KeyAttachedTo == intToKeys[i])
                return std::unique_ptr<KeyProvider>(new KeyProvider(i));
        return nullptr;
    }
    
    double calculateValue() override
    {
      return static_cast<float>(utility::Keyboard.isKeyPressed(this->m_KeyAttachedTo));
    }

public:
    KeyProvider(const int attachedKeyId) 
    {
      this->m_KeyAttachedTo = this->getKeyById(attachedKeyId);
    }

protected:
    sf::Keyboard::Key getKeyById(int id)
    {
      return intToKeys[id];
    }
};

#endif