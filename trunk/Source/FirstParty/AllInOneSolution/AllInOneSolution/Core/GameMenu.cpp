#include "GameMenu.hpp"
#include "resources/ResourceManager.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

GameMenu::GameMenu(sf::Vector2f position, std::string caption, int Buttons, sf::RenderWindow& screen, ResourceManager& resourceManager) :
Menu(position, screen, resourceManager),
m_menuCaption(caption, sf::Vector2f(0, 0), 0, resourceManager.getBitmapFont("gui_large"), LineLabel::Centered),
m_playAgain(sf::Vector2f(0, 0), "Play Again", "buttonIdleFont", resourceManager, "ButtonIdle", sf::Vector2f(0, -2)),
m_mainMenu(sf::Vector2f(0, 0), "Main Menu", "buttonIdleFont", resourceManager, "ButtonIdle", sf::Vector2f(0, -2)),
m_nextLevel(sf::Vector2f(0, 0), "Next Level", "buttonIdleFont", resourceManager, "ButtonIdle", sf::Vector2f(0, -2)),
m_continue(sf::Vector2f(0, 0), "Continue", "buttonIdleFont", resourceManager, "ButtonIdle", sf::Vector2f(0, -2))
{
    m_activButtons = Buttons;

    m_menuBackround.setTexture(*resourceManager.getTexture("Window"));
    m_menuBackround.setPosition(m_position);

    m_size = sf::Vector2i(m_menuBackround.getTextureRect().width, m_menuBackround.getTextureRect().height);

    m_menuCaption.setPosition(m_position.x + m_size.x / 2.f, m_position.y + 40);

    int distance = 100;

    if(PlayAgain == static_cast<ButtonFlag>(m_activButtons & static_cast<int>(PlayAgain)))
    {
        m_playAgain.setPosition(sf::Vector2f(m_position.x + m_size.x / 2.f - m_playAgain.getSize().x / 2.f, m_menuBackround.getPosition().y + distance));
        m_playAgain.bindHover("ButtonHover", "buttonHoverFont", sf::Vector2f(0, 0), sf::Vector2f(0, -2));
        m_playAgain.bindPressed("ButtonPressed", "buttonHoverFont", sf::Vector2f(0, 0), sf::Vector2f(10, 5));
        distance += 60;
    }
    if(Continue == static_cast<ButtonFlag>(m_activButtons & static_cast<int>(Continue)))
    {
        m_continue.setPosition(sf::Vector2f(m_position.x + m_size.x / 2.f - m_continue.getSize().x / 2.f, m_menuBackround.getPosition().y + distance));
        m_continue.bindHover("ButtonHover", "buttonHoverFont", sf::Vector2f(0, 0), sf::Vector2f(0, -2));
        m_continue.bindPressed("ButtonPressed", "buttonHoverFont", sf::Vector2f(0, 0), sf::Vector2f(10, 5));
        distance += 60;
    }
    if(NextLevel == static_cast<ButtonFlag>(m_activButtons & static_cast<int>(NextLevel)))
    {
        m_nextLevel.setPosition(sf::Vector2f(m_position.x + m_size.x / 2.f - m_nextLevel.getSize().x / 2.f, m_menuBackround.getPosition().y + distance));
        m_nextLevel.bindHover("ButtonHover", "buttonHoverFont", sf::Vector2f(0, 0), sf::Vector2f(0, -2));
        m_nextLevel.bindPressed("ButtonPressed", "buttonHoverFont", sf::Vector2f(0, 0), sf::Vector2f(10, 5));
        distance += 60;
    }
    if(MainMenu == static_cast<ButtonFlag>(m_activButtons & static_cast<int>(MainMenu)))
    {
        m_mainMenu.setPosition(sf::Vector2f(m_position.x + m_size.x / 2.f - m_mainMenu.getSize().x / 2.f, m_menuBackround.getPosition().y + distance));
        m_mainMenu.bindHover("ButtonHover", "buttonHoverFont", sf::Vector2f(0, 0), sf::Vector2f(0, -5));
        m_mainMenu.bindPressed("ButtonPressed", "buttonHoverFont", sf::Vector2f(0, 0), sf::Vector2f(10, 5));
        distance += 60;
    }
}

void GameMenu::draw(const DrawParameter& params)
{
    params.getTarget().draw(m_menuBackround);
    m_menuCaption.draw(params);

    if(PlayAgain == static_cast<ButtonFlag>(m_activButtons & static_cast<int>(PlayAgain)))
        m_playAgain.draw(params);

    if(NextLevel == static_cast<ButtonFlag>(m_activButtons & static_cast<int>(NextLevel)))
        m_nextLevel.draw(params);

    if(MainMenu == static_cast<ButtonFlag>(m_activButtons & static_cast<int>(MainMenu)))
        m_mainMenu.draw(params);

    if(Continue == static_cast<ButtonFlag>(m_activButtons & static_cast<int>(Continue)))
        m_continue.draw(params);
}

void GameMenu::update(const sf::RenderWindow& screen)
{
    if(PlayAgain == static_cast<ButtonFlag>(m_activButtons & static_cast<int>(PlayAgain)))
        m_playAgain.update(screen);
    if(NextLevel == static_cast<ButtonFlag>(m_activButtons & static_cast<int>(NextLevel)))
        m_nextLevel.update(screen);
    if(MainMenu == static_cast<ButtonFlag>(m_activButtons & static_cast<int>(MainMenu)))
        m_mainMenu.update(screen);
    if(Continue == static_cast<ButtonFlag>(m_activButtons & static_cast<int>(Continue)))
        m_continue.update(screen);
}

bool GameMenu::isPressed(ButtonFlag button)
{
    if(PlayAgain == static_cast<ButtonFlag>(m_activButtons & static_cast<int>(button)))
        return m_playAgain.isPressed();
    if(NextLevel == static_cast<ButtonFlag>(m_activButtons & static_cast<int>(button)))
        return m_nextLevel.isPressed();
    if(MainMenu == static_cast<ButtonFlag>(m_activButtons & static_cast<int>(button)))
        return m_mainMenu.isPressed();
    if(Continue == static_cast<ButtonFlag>(m_activButtons & static_cast<int>(button)))
        return m_continue.isPressed();

    return false;
}

void GameMenu::setPosition(sf::Vector2f position)
{
    m_position = position;

    rebuild();
}

void GameMenu::rebuild()
{
    m_menuBackround.setPosition(m_position);

    m_size = sf::Vector2i(m_menuBackround.getTextureRect().width, m_menuBackround.getTextureRect().height);

    m_menuCaption.setPosition(m_position.x + m_size.x / 2.f, m_position.y + 40);

    int distance = 100;

    if(PlayAgain == static_cast<ButtonFlag>(m_activButtons & static_cast<int>(PlayAgain)))
    {
        m_playAgain.setPosition(sf::Vector2f(m_position.x + m_size.x / 2.f - m_playAgain.getSize().x / 2.f, m_menuBackround.getPosition().y + distance));
        m_playAgain.bindHover("ButtonHover", "buttonHoverFont", sf::Vector2f(0, 0), sf::Vector2f(0, -2));
        m_playAgain.bindPressed("ButtonPressed", "buttonHoverFont", sf::Vector2f(0, 0), sf::Vector2f(10, 5));
        distance += 60;
    }
    if(Continue == static_cast<ButtonFlag>(m_activButtons & static_cast<int>(Continue)))
    {
        m_continue.setPosition(sf::Vector2f(m_position.x + m_size.x / 2.f - m_continue.getSize().x / 2.f, m_menuBackround.getPosition().y + distance));
        m_continue.bindHover("ButtonHover", "buttonHoverFont", sf::Vector2f(0, 0), sf::Vector2f(0, -2));
        m_continue.bindPressed("ButtonPressed", "buttonHoverFont", sf::Vector2f(0, 0), sf::Vector2f(10, 5));
        distance += 60;
    }
    if(NextLevel == static_cast<ButtonFlag>(m_activButtons & static_cast<int>(NextLevel)))
    {
        m_nextLevel.setPosition(sf::Vector2f(m_position.x + m_size.x / 2.f - m_playAgain.getSize().x / 2.f, m_menuBackround.getPosition().y + distance));
        m_nextLevel.bindHover("ButtonHover", "buttonHoverFont", sf::Vector2f(0, 0), sf::Vector2f(0, -2));
        m_nextLevel.bindPressed("ButtonPressed", "buttonHoverFont", sf::Vector2f(0, 0), sf::Vector2f(10, 5));
        distance += 60;
    }
    if(MainMenu == static_cast<ButtonFlag>(m_activButtons & static_cast<int>(MainMenu)))
    {
        m_mainMenu.setPosition(sf::Vector2f(m_position.x + m_size.x / 2.f - m_playAgain.getSize().x / 2.f, m_menuBackround.getPosition().y + distance));
        m_mainMenu.bindHover("ButtonHover", "buttonHoverFont", sf::Vector2f(0, 0), sf::Vector2f(0, -5));
        m_mainMenu.bindPressed("ButtonPressed", "buttonHoverFont", sf::Vector2f(0, 0), sf::Vector2f(10, 5));
        distance += 60;
    }
}
sf::Vector2f GameMenu::getSize()
{
    return sf::Vector2f(static_cast<float>(m_menuBackround.getTextureRect().width),
                        static_cast<float>(m_menuBackround.getTextureRect().height));
}
