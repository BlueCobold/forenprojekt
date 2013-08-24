#include "Menu.hpp"

#include "Button.hpp"
#include "CheckBox.hpp"
#include "LineLabel.hpp"
#include "MenuSprite.hpp"
#include "Slider.hpp"
#include "SubWindow.hpp"
#include "../resources/ResourceManager.hpp"

#include <algorithm>

#include <SFML/Graphics/RenderWindow.hpp>

Menu::Menu(const MenuTemplate& menuTemplate,
           const sf::Vector2f& position,
           sf::RenderWindow& screen) :
        m_position(position),
        m_screen(screen),
        m_template(menuTemplate),
        m_panel(menuTemplate.menuElements, position)
{
    m_template.background.setPosition(m_position);

    m_size = sf::Vector2i(m_template.background.getTextureRect().width, m_template.background.getTextureRect().height);

    for(auto info = begin(menuTemplate.subWindow); info != end(menuTemplate.subWindow); ++info)
    {
        std::unique_ptr<SubWindow> subWindow(new SubWindow(info->id, m_position, info->size, info->position, info->innerHeight, info->menuElements, info->style));
        m_panel.add(std::move(subWindow));
    }
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

    m_template.background.setPosition(m_position);

    m_panel.setPosition(position);
}

const sf::Vector2f& Menu::getPosition() const
{
    return m_position;
}

void Menu::draw(const DrawParameter& params)
{
    params.getTarget().draw(m_template.background);
    drawAdditionalBackground(params);
    m_panel.draw(params);
    drawAdditionalForeground(params);
}

void Menu::drawAdditionalBackground(const DrawParameter& params)
{
}

void Menu::drawAdditionalForeground(const DrawParameter& params)
{
    m_panel.drawAdditionalForeground(params);
}

void Menu::update(const sf::RenderWindow& screen)
{
    m_panel.update(screen);
}

void Menu::registerOnClick(std::function<void(const Button& sender)> callback)
{
    m_panel.registerOnClick(callback);
}

sf::RenderWindow& Menu::getRenderWindow() const
{
    return m_screen;
}

template<class T>
T& Menu::find(int id, const MenuElementType::Type type, const std::string& errorKey) const
{
    try
    {
        return m_panel.find<T>(id, type, errorKey);
    }
    catch(...)
    {
        for(auto it = m_template.subWindow.begin(); it != m_template.subWindow.end(); ++it)
        {
            MenuPanel* panel = m_panel.find<SubWindow>(it->id, MenuElementType::SubWindow, "SubWindowId").getPanel();
            try
            {
                return panel->find<T>(id, type, errorKey);
            }
            catch(...)
            {
                continue;
            }
        }
    }
    throw std::runtime_error(utility::replace(utility::translateKey(errorKey), utility::toString(id)));
}

CheckBox& Menu::getCheckbox(int id) const
{
    return find<CheckBox>(id, MenuElementType::CheckBox, "CheckboxId");
}

Slider& Menu::getSlider(int id) const
{
    return find<Slider>(id, MenuElementType::Slider, "sliderID");
}

LineLabel& Menu::getLabel(int id) const
{
    return find<LineLabel>(id, MenuElementType::Label, "LineLabelId");
}

MenuSprite& Menu::getSprite(int id) const
{
    return find<MenuSprite>(id, MenuElementType::Image, "MenuSpriteId");
}

Button& Menu::getButton(int id) const
{
    return find<Button>(id, MenuElementType::Button, "ButtonId");
}

void Menu::changeIdleSprite(const int id, const sf::Sprite& sprite)
{
    getButton(id).changeIdleSprite(sprite);
}

void Menu::changeHoverSprite(const int id, const sf::Sprite& sprite)
{
    getButton(id).changeHoverSprite(sprite);
}

void Menu::changePressedSprite(const int id, const sf::Sprite& sprite)
{
    getButton(id).changePressedSprite(sprite);
}
