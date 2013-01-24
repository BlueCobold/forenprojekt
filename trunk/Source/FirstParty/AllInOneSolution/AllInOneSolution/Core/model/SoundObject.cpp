#include "SoundObject.hpp"

void SoundObject::bindSoundBuffer(sf::SoundBuffer* soundBuffer)
{
    m_sound.setBuffer(*soundBuffer);
}


void SoundObject::play()
{
    m_sound.play();
}

void SoundObject::pause()
{
    m_sound.pause();
}

void SoundObject::stop()
{
    m_sound.stop();
}