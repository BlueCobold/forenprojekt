#include "SoundObject.hpp"

SoundObject::SoundObject()
{
}

SoundObject::~SoundObject()
{

} 


void SoundObject::bindSound(sf::Music& sound)
{
    m_sound = sound;
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