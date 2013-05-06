#include "Menu.hpp"
#include "Button.hpp"
#include "../resources/ResourceManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

Menu::Menu(
    const sf::Vector2f& position,
    sf::RenderWindow& screen,
    ResourceManager& resourceManager,
    const std::string& backgroundName) :
        m_position(position),
        m_screen(screen),
        m_resourceManager(resourceManager)
{
    m_background.setTexture(*resourceManager.getTexture(backgroundName));
    m_background.setPosition(m_position);

    m_size = sf::Vector2i(m_background.getTextureRect().width, m_background.getTextureRect().height);
}

Menu::~Menu()
{}

const sf::Vector2i& Menu::getSize() const
{
    return m_size;
}

void Menu::setPosition(const sf::Vector2f& position)
{
    m_position = position;
    
    m_background.setPosition(m_position);

    for(auto it = begin(m_buttons); it != end(m_buttons); ++it)
        (*it)->setPosition(sf::Vector2f(position.x - (*it)->getSize().x / 2, position.y));
}

const sf::Vector2f& Menu::getPosition() const
{
    return m_position;
}

void Menu::draw(const DrawParameter& params)
{
    params.getTarget().draw(m_background);
    for(auto it = begin(m_buttons); it != end(m_buttons); ++it)
        (*it)->draw(params);
}

void Menu::update(const sf::RenderWindow& screen)
{
    for(auto it = begin(m_buttons); it != end(m_buttons); ++it)
        (*it)->update(screen);
}

void Menu::createButton(const sf::Vector2f& position, const sf::Vector2f& offset, const std::string& text, const int id)
{
    sf::Sprite image(*m_resourceManager.getTexture("ButtonIdle"));
    auto pos = position;
    pos.x -= image.getTextureRect().width / 2;
    std::unique_ptr<Button> button(new Button(pos, text, m_resourceManager.getBitmapFont("buttonIdleFont"), image, id, offset, sf::Vector2f(0, -2)));

    sf::Sprite hover(*m_resourceManager.getTexture("ButtonHover"));
    button->bindHover(hover, m_resourceManager.getBitmapFont("buttonHoverFont"), sf::Vector2f(0, 0), sf::Vector2f(0, 0));
    
    sf::Sprite pressed(*m_resourceManager.getTexture("ButtonPressed"));
    button->bindPressed(pressed, m_resourceManager.getBitmapFont("buttonHoverFont"), sf::Vector2f(-6, -6), sf::Vector2f(1, 1));

    button->registerOnPressed([this](const Button& sender)
    {
        if(m_clickCallback != nullptr)
            m_clickCallback(sender);
    });

    m_buttons.push_back(std::move(button));
}

void Menu::registerOnClick(std::function<void(const Button& sender)> callback)
{
    m_clickCallback = callback;
}
