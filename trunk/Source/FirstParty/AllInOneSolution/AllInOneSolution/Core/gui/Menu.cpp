#include "Menu.hpp"
#include "Button.hpp"
#include "../resources/ResourceManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

Menu::Menu(const MenuTemplate& menuTemplate,
           const sf::Vector2f& position,
           sf::RenderWindow& screen) :
        m_position(position),
        m_screen(screen),
        m_template(menuTemplate)
{
    m_template.background.setPosition(m_position);

    m_size = sf::Vector2i(m_template.background.getTextureRect().width, m_template.background.getTextureRect().height);

    for(auto button = begin(m_template.buttons); button != end(m_template.buttons); ++button)
        createButton(*button);
}

Menu::~Menu()
{}

MenuTemplate& Menu::getTemplate()
{
    return m_template;
}

const sf::Vector2i& Menu::getSize() const
{
    return m_size;
}

void Menu::setPosition(const sf::Vector2f& position)
{
    m_position = position;

    m_template.background.setPosition(m_position);

    for(auto it = begin(m_buttons); it != end(m_buttons); ++it)
        (*it)->setPosition(m_position);
}

const sf::Vector2f& Menu::getPosition() const
{
    return m_position;
}

void Menu::draw(const DrawParameter& params)
{
    params.getTarget().draw(m_template.background);
    for(auto it = begin(m_buttons); it != end(m_buttons); ++it)
        (*it)->draw(params);
}

void Menu::update(const sf::RenderWindow& screen)
{
    for(auto it = begin(m_buttons); it != end(m_buttons); ++it)
        (*it)->update(screen);
}

void Menu::createButton(const ButtonInfo& info)
{
    std::unique_ptr<Button> button(new Button(info.id, info.style, m_position, info.position));

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
