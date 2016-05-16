#include "Goody.hpp"

#ifdef TOUCHSIM
Goody::Goody(const sf::Keyboard::Key key,
             const Type type,
             const sf::FloatRect& touchArea,
             const float durationTime,
             const float cooldownTime,
             const int charges) :
    m_button(touchArea),
    m_key(key),
    m_active(false),
    m_charges(charges),
    m_cooldownTime(cooldownTime),
    m_nextUseTime(0),
    m_durationTime(durationTime),
    m_durationUntilTime(0),
    m_selected(false),
    m_type(type),
    m_callback(nullptr)
{
}
#else
Goody::Goody(const sf::Keyboard::Key key,
             const Type type,
             const float durationTime,
             const float cooldownTime,
             const int charges) :
    m_key(key),
    m_active(false),
    m_charges(charges),
    m_cooldownTime(cooldownTime),
    m_nextUseTime(0),
    m_durationTime(durationTime),
    m_durationUntilTime(0),
    m_selected(false),
    m_type(type),
    m_callback(nullptr)
{
}
#endif

void Goody::update(const float elapsedTime)
{
#ifdef TOUCHSIM
    m_button.update();
    bool down = utility::Keyboard.isKeyDown(m_key) || (m_selected && utility::Mouse.leftButtonDown()) || m_button.isDown();
    bool pressed = utility::Keyboard.isKeyPressed(m_key) || (m_selected && utility::Mouse.leftButtonPressed()) || m_button.isPressed();
#else
    bool down = utility::Keyboard.isKeyDown(m_key) || (m_selected && utility::Mouse.leftButtonDown());
    bool pressed = utility::Keyboard.isKeyPressed(m_key) || (m_selected && utility::Mouse.leftButtonPressed());
#endif
    if(down && m_charges != 0 && m_nextUseTime < elapsedTime)
    {
        if(canActivate())
            m_active = true;
        if(m_charges > 0)
            m_charges--;
        if(m_cooldownTime > 0)
            m_nextUseTime = elapsedTime + m_cooldownTime;
        if(m_durationTime > 0)
            m_durationUntilTime = elapsedTime + m_durationTime;

        if(m_callback != nullptr)
            m_callback(*this);
    }
    else if(pressed && m_charges == -1 && m_nextUseTime < elapsedTime)
    {
        if(canActivate())
            m_active = true;

        if(m_callback != nullptr)
            m_callback(*this);
    }
    else if(m_durationUntilTime < elapsedTime)
        m_active = false;
}

bool Goody::isActive() const
{
    return m_active;
}

bool Goody::isSelected() const
{
    return m_selected;
}

bool Goody::canActivate() const
{
    return true;
}

int Goody::getCharges() const
{
    return m_charges;
}

void Goody::setSelected(const bool selected)
{
    m_selected = selected;
}

const Goody::Type Goody::getType() const
{
    return m_type;
}

void Goody::registerForActivation(std::function<void(Goody& sender)> callback)
{
    m_callback = callback;
}

void Goody::setCharges(const int charges)
{
    m_charges = charges;
}
