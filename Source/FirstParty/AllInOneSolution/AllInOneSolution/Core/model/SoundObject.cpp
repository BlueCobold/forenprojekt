#include "SoundObject.hpp"

#include <stdio.h>

SoundObject::SoundObject()
    : m_sound(nullptr)
{
}

SoundObject::~SoundObject()
{
    if(m_sound != nullptr)
    {
        m_sound = nullptr;
    }
} 


void SoundObject::bindSound(sf::Sound* sound)
{
    m_sound = sound;
}


void SoundObject::play()
{
    m_sound->play();
}

void SoundObject::pause()
{
    m_sound->pause();
}

void SoundObject::stop()
{
    m_sound->stop();
}