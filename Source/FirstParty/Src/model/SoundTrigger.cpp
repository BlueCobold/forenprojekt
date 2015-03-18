#include "SoundTrigger.hpp"

SoundTrigger::SoundTrigger(const std::string& key, 
                           SoundManager& soundManager,
                           std::unique_ptr<ValueProvider> trigger) :
    m_sound(key, soundManager),
    m_trigger(std::move(trigger))
{
}

SoundTrigger::SoundTrigger(SoundTrigger& other) :
    m_sound(other.m_sound),
    m_trigger(std::move(other.m_trigger))
{
}
SoundTrigger::SoundTrigger(SoundTrigger&& other) :
    m_sound(other.m_sound),
    m_trigger(std::move(other.m_trigger))
{
}
void SoundTrigger::update()
{
    if(m_trigger->getValue() >= 1)
        m_sound.play(100.f);
}