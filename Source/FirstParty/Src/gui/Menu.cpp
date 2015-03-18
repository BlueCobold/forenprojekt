#include "Menu.hpp"

#include "Button.hpp"
#include "CheckBox.hpp"
#include "LineLabel.hpp"
#include "MenuSprite.hpp"
#include "Slider.hpp"
#include "SubWindow.hpp"
#include "InputBox.hpp"
#include "../resources/ResourceManager.hpp"

#include <algorithm>

#include <SFML/Graphics/RenderWindow.hpp>

const float Menu::Left = 0.0f;
const float Menu::Right = 1.0f;
const float Menu::Center= 0.5f;
const float Menu::Top = 0.0f;
const float Menu::Middle = 0.5f;
const float Menu::Bottom = 1.0f;

Menu::Menu(MenuTemplate& menuTemplate,
           sf::RenderWindow& screen) :
           m_offset(menuTemplate.menuOffset),
           m_screen(&screen),
           m_horizontalPercentage(menuTemplate.horizontalPercentage),
           m_verticalPercentage(menuTemplate.verticalPercentage),
           m_currentPosition(sf::Vector2f(screen.getSize().x * menuTemplate.horizontalPercentage, 
                                          screen.getSize().y * menuTemplate.verticalPercentage) + menuTemplate.menuOffset),
           m_panel(menuTemplate.menuElements, m_currentPosition)
           
{
    m_template = (std::move(menuTemplate));
    m_template.background.setPosition(m_currentPosition);

    m_size = sf::Vector2i(m_template.background.getTextureRect().width, m_template.background.getTextureRect().height);

    for(auto info = begin(m_template.subWindow); info != end(m_template.subWindow); ++info)
    {
        std::unique_ptr<SubWindow> subWindow(new SubWindow(info->id, m_currentPosition, info->size, info->position, info->innerHeight, info->menuElements, info->style));
        m_panel.add(std::move(subWindow));
    }
}

Menu::~Menu()
{}

const sf::Vector2i& Menu::getSize() const
{
    return m_size;
}

void Menu::setPosition(const sf::Vector2f& offset,
                       float horizontalPercentage,
                       float verticalPercentage)
{
    m_offset = offset;
    m_verticalPercentage = verticalPercentage;
    m_horizontalPercentage = horizontalPercentage;
    m_currentPosition = sf::Vector2f(m_screen->getSize().x * m_horizontalPercentage, 
                                     m_screen->getSize().y * m_verticalPercentage) + m_offset;

    m_template.background.setPosition(m_currentPosition);
    m_panel.setPosition(m_currentPosition);
}

const sf::Vector2f& Menu::getPosition() const
{
    return m_currentPosition;
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

void Menu::update(sf::RenderWindow& screen, const float time)
{
    m_screen = &screen;

    m_currentPosition = sf::Vector2f(m_screen->getSize().x * m_horizontalPercentage, 
                                     m_screen->getSize().y * m_verticalPercentage) + m_offset;

    updateLayout();
    m_panel.update(screen, time);
}

void Menu::update(sf::RenderWindow& screen, const float time, const MenuElementType::Type type)
{
    m_screen = &screen;

    m_currentPosition = sf::Vector2f(m_screen->getSize().x * m_horizontalPercentage, 
                                     m_screen->getSize().y * m_verticalPercentage) + m_offset;

    updateLayout();
    m_panel.update(screen, time, type);
}

void Menu::registerOnClick(std::function<void(const Button& sender)> callback)
{
    m_panel.registerOnClick(callback);
}

sf::RenderWindow& Menu::getRenderWindow() const
{
    return *m_screen;
}

template<class T>
T* Menu::find(int id, const MenuElementType::Type type) const
{
    if(auto result = m_panel.find<T>(id, type))
        return result;
    else
    {
        for(auto it = m_template.subWindow.begin(); it != m_template.subWindow.end(); ++it)
        {
            MenuPanel* panel = m_panel.find<SubWindow>(it->id, MenuElementType::SubWindow)->getPanel();
            if(auto result2 = panel->find<T>(id, type))
                return result2;
        }
    }
    return nullptr;
}

CheckBox& Menu::getCheckbox(int id) const
{
    return *find<CheckBox>(id, MenuElementType::CheckBox);
}

Slider& Menu::getSlider(int id) const
{
    return *find<Slider>(id, MenuElementType::Slider);
}

LineLabel& Menu::getLabel(int id) const
{
    return *find<LineLabel>(id, MenuElementType::Label);
}

MenuSprite& Menu::getSprite(int id) const
{
    return *find<MenuSprite>(id, MenuElementType::Image);
}

Button& Menu::getButton(int id) const
{
    return *find<Button>(id, MenuElementType::Button);
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

InputBox& Menu::getInputBox(int id) const
{
    return *find<InputBox>(id, MenuElementType::InputBox);
}

void Menu::updateLayout()
{
    m_currentPosition = sf::Vector2f(m_screen->getSize().x * m_horizontalPercentage, 
                                     m_screen->getSize().y * m_verticalPercentage) + m_offset;
    m_template.background.setPosition(m_currentPosition);
    m_panel.updateLayout(m_currentPosition);
}