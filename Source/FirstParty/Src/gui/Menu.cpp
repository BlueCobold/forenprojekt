#include "Menu.hpp"

#include "AnimationContainer.hpp"
#include "Button.hpp"
#include "CheckBox.hpp"
#include "CustomContent.hpp"
#include "LineLabel.hpp"
#include "MenuSprite.hpp"
#include "InputBox.hpp"
#include "InteractiveLabel.hpp"
#include "Slider.hpp"
#include "SubWindow.hpp"
#include "../resources/ResourceManager.hpp"

#include <algorithm>

#include <SFML/Graphics/RenderWindow.hpp>

Menu::Menu(const MenuTemplate& menuTemplate, sf::RenderWindow& screen) :
           m_screen(&screen),
           m_panel(menuTemplate.menuElements),
           m_template(menuTemplate),
           m_zoomFactor(1.0f)
{
    update(screen, 0);
}

Menu::~Menu()
{}

void Menu::doDraw(const DrawParameter& params)
{
    auto view = params.getTarget().getView();
    view.zoom(m_zoomFactor);
    params.getTarget().setView(view);

    params.getTarget().draw(m_template.background);
    drawAdditionalBackground(params);
    m_panel.draw(params);
    drawAdditionalForeground(params);

    view.zoom(1.f);
    params.getTarget().setView(view);
}

void Menu::drawAdditionalBackground(const DrawParameter& params)
{ }

void Menu::drawAdditionalForeground(const DrawParameter& params)
{
    m_panel.drawAdditionalForeground(params);
}

void Menu::update(sf::RenderWindow& screen, const double time)
{
    onBeforeUpdate(screen, time);
    m_panel.update(screen, time);
    updated(screen, time);
}

void Menu::onBeforeUpdate(sf::RenderWindow& screen, const double time)
{ }

void Menu::updated(sf::RenderWindow& screen, const double time)
{ }

void Menu::update(sf::RenderWindow& screen, const double time, const MenuElementType::Type type)
{
    m_panel.update(screen, time, type);
}

void Menu::registerOnClick(std::function<void(const Button& sender)> callback)
{
    m_panel.registerOnClick(callback);
}

template<class T>
T* Menu::find(int id, const MenuElementType::Type type) const
{
    if(auto result = m_panel.find<T>(id, type))
        return result;
    else
    {
        for(auto it = begin(m_panel.getElements()); it != end(m_panel.getElements()); ++it)
        {
            if((*it)->getType() != MenuElementType::SubWindow)
                continue;

            if(auto result2 = dynamic_cast<SubWindow*>((*it).get())->getPanel()->find<T>(id, type))
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

InteractiveLabel& Menu::getInteractiveLabel(int id) const
{
    return *find<InteractiveLabel>(id, MenuElementType::InteractiveLabel);
}

MenuSprite& Menu::getSprite(int id) const
{
    return *find<MenuSprite>(id, MenuElementType::Image);
}

AnimationContainer& Menu::getAnimation(int id) const
{
    return *find<AnimationContainer>(id, MenuElementType::Animation);
}

CustomContent& Menu::getCustomContent(int id) const
{
    return *find<CustomContent>(id, MenuElementType::CustomContent);
}

Button& Menu::getButton(int id) const
{
    return *find<Button>(id, MenuElementType::Button);
}

InputBox& Menu::getInputBox(int id) const
{
    return *find<InputBox>(id, MenuElementType::InputBox);
}

SubWindow& Menu::getSubWindow(int id) const
{
    return *find<SubWindow>(id, MenuElementType::SubWindow);
}

void Menu::setZoomFactor(float zoomFactor)
{
    m_zoomFactor = zoomFactor;

    for(auto it = begin(m_panel.getElements()); it != end(m_panel.getElements()); ++it)
    {
        if((*it)->getType() == MenuElementType::SubWindow)
            dynamic_cast<SubWindow*>((*it).get())->setZoomFactor(m_zoomFactor);
    }
}
