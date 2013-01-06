#include "SoundObject.hpp"

void SoundObject::bindSound(sf::Music* sound)
{
    m_sound = std::unique_ptr<sf::Music>(sound);
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