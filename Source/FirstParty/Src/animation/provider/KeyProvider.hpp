#pragma once

#ifndef KEY_PROVIDER_HPP
#define KEY_PROVIDER_HPP

#include <SFML/Window/Keyboard.hpp>

#include "ValueProvider.hpp"
#include "../../Input.hpp"

using namespace sf;

class KeyProvider : public ValueProvider
{
private:
    Keyboard::Key m_KeyAttachedTo;

    // The array uses the US-ASCII encoding.
    const static Keyboard::Key intToKeys[128];

public:

    KeyProvider(const int attachedKeyId) 
    {
      this->m_KeyAttachedTo = this->getKeyById(attachedKeyId);
    }

    virtual float getValue() override
    {
      return static_cast<float>(utility::Keyboard.isKeyPressed(this->m_KeyAttachedTo));
    }

    virtual std::unique_ptr<ValueProvider> clone() const override
    {
        for(int i=0; i<128; i++)
            if(m_KeyAttachedTo == intToKeys[i])
                return std::unique_ptr<KeyProvider>(new KeyProvider(i));
        return nullptr;
    }

protected:

    Keyboard::Key getKeyById(int id)
    {
      return intToKeys[id];
    }
};

#endif