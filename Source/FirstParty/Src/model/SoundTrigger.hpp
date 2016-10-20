#pragma once

#ifndef SOUNDTRIGGER_HPP
#define SOUNDTRIGGER_HPP

#include "SoundObject.hpp"
#include "../animation/provider/ValueProvider.hpp"

class SoundTrigger
{
public:
    SoundTrigger(const std::string& key,
                 SoundManager& soundManager,
                 std::unique_ptr<ValueProvider> trigger);

    SoundTrigger(SoundTrigger& other);
    SoundTrigger(SoundTrigger&& other);

    void update();

private:
    SoundObject m_sound;
    std::unique_ptr<ValueProvider> m_trigger;
};

#endif // SOUNDTRIGGER_HPP
