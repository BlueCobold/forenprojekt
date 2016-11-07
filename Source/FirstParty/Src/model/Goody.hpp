#pragma once

#ifndef GOODY_HPP
#define GOODY_HPP

#include "../Input.hpp"
#include "../TouchButton.hpp"

#include <functional>

class Goody
{
public:
    enum Type {GravityGoody,
               InvulnerableGoody,
               ExtraBallGoody,
               ExtraTimeGoody,
               // Counter must allways be the last entry
               Counter};

#ifdef TOUCHSIM
    Goody(const sf::Keyboard::Key key, const Type type, const sf::FloatRect& touchArea, const double durationTime = 0,
        const double cooldownTime = 0, const int charges = -1);
#else
    Goody(const sf::Keyboard::Key key, const Type type, const double durationTime = 0,
          const double cooldownTime = 0, const int charges = -1);
#endif

    void update(const double elapsedTime);

    bool isActive() const;
    bool canActivate() const;
    bool isSelected() const;
    int getCharges() const;
    void setCharges(const int charges);
    const Type getType() const;
    void setSelected(const bool selected);
    void registerForActivation(std::function<void(Goody& sender)> callback);

protected:
    double getCurrentTime();

private:
#ifdef TOUCHSIM
    TouchButton m_button;
#endif
    sf::Keyboard::Key m_key;
    bool m_active;
    int m_charges;
    double m_currentTime;
    double m_cooldownTime;
    double m_nextUseTime;
    double m_durationTime;
    double m_durationUntilTime;
    bool m_selected;
    Type m_type;
    std::function<void(Goody& sender)>m_callback;

    virtual bool calculateCanActivate() const;
    virtual void updated();
};

#endif
