#include "KeyboardMenu.hpp"

#include "../resources/ResourceManager.hpp"
#include "Button.hpp"

KeyboardMenu::KeyboardMenu(sf::RenderWindow& screen,
                           ResourceManager& resourceManager) :
    Menu(*resourceManager.getMenuTemplate("KeyboardMenu"), screen),
    m_showKeyboard(false),
    m_shiftKeyPressed(false),
    m_lastKey(-1)
{
}

void KeyboardMenu::showKeyboard()
{
    m_showKeyboard = true;
}

void KeyboardMenu::doDraw(const DrawParameter& params)
{
    if(!m_showKeyboard)
        return;

    Menu::doDraw(params);
}

void KeyboardMenu::updated(sf::RenderWindow& screen, const double time)
{
    if(m_lastKey != -1)
    {
        utility::Keyboard.notifyKeyReleased(static_cast<sf::Keyboard::Key>(m_lastKey));
        if(m_shiftKeyPressed)
        {
            m_shiftKeyPressed = false;
            utility::Keyboard.notifyKeyReleased(sf::Keyboard::LShift);
        }
    }

    if(!m_showKeyboard)
        return;

    int clicked = -1;
    registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    
    if(clicked == sf::Keyboard::LShift || clicked == sf::Keyboard::RShift)
    {
        m_shiftKeyPressed = !m_shiftKeyPressed;
        if(!m_shiftKeyPressed)
            utility::Keyboard.notifyKeyReleased(sf::Keyboard::LShift);
    }
    else if(clicked == BUTTON_KEYBOARD_OFF)
    {
        m_showKeyboard = false;
        m_shiftKeyPressed = false;
    }
    else if(clicked != -1)
    {
        utility::Keyboard.notifyKeyPressed(static_cast<sf::Keyboard::Key>(clicked));
        m_lastKey = clicked;
    }
    else
        m_lastKey = -1;

    if(m_shiftKeyPressed)
        utility::Keyboard.notifyKeyPressed(sf::Keyboard::LShift);
}

bool KeyboardMenu::isShown()
{
    return m_showKeyboard;
}