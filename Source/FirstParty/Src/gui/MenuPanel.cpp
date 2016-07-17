#include "MenuPanel.hpp"

#include "Button.hpp"
#include "CheckBox.hpp"
#include "LineLabel.hpp"
#include "MenuSprite.hpp"
#include "Slider.hpp"
#include "InputBox.hpp"
#include "AnimationContainer.hpp"
#include "InteractiveLabel.hpp"

MenuPanel::MenuPanel(const std::vector<std::unique_ptr<MenuElement>>& elements)
{
    for(auto element = begin(elements); element != end(elements); ++element)
    {
        auto clone = (*element)->clone();
        if(clone->getType() == MenuElementType::Button)
        {
            dynamic_cast<Button*>(clone.get())->registerOnPressed([this](const Button& sender)
            {
                if(m_clickCallback != nullptr)
                    m_clickCallback(sender);
            });
        }
        m_elements.push_back(std::move(clone));
    }

    setCorrelation();
    std::sort(m_elements.begin(), m_elements.end(), 
        [](const std::unique_ptr<MenuElement>& a, const std::unique_ptr<MenuElement>& b) -> bool
    { 
        return a->getId() < b->getId(); 
    });
}

void MenuPanel::add(std::unique_ptr<MenuElement> element)
{
    m_elements.push_back(std::move(element));
    std::sort(m_elements.begin(), m_elements.end(), 
        [](const std::unique_ptr<MenuElement>& a, const std::unique_ptr<MenuElement>& b) -> bool
    { 
        return a->getId() < b->getId(); 
    });
}

void MenuPanel::draw(const DrawParameter& params)
{
    for(auto it = begin(m_elements); it != end(m_elements); ++it)
        (*it)->draw(params);
}

void MenuPanel::update(const sf::RenderWindow& screen, const float time, const sf::Vector2i& mouseOffset)
{
    for(auto it = begin(m_elements); it != end(m_elements); ++it)
        (*it)->update(screen, time, mouseOffset);
}

void MenuPanel::update(const sf::RenderWindow& screen, const float time, const MenuElementType::Type type, const sf::Vector2i& mouseOffset)
{
    for(auto it = begin(m_elements); it != end(m_elements); ++it)
    {
        if(it->get()->getType() == type)
            (*it)->update(screen, time, mouseOffset);
    }
}

void MenuPanel::setCorrelation()
{
    for(auto s = begin(m_elements); s != end(m_elements); ++s)
    {
        auto id = s->get()->getVisibleWhenId();
        if(id == -1 || s->get()->getId() == id)
            continue;

        for(auto it = begin(m_elements); it != end(m_elements); ++it)
            if((*it)->getId() == id)
                s->get()->setVisibleWhenSubject(it->get());
    }
}

void MenuPanel::registerOnClick(std::function<void(const Button& sender)> callback)
{
    m_clickCallback = callback;
}

void MenuPanel::drawAdditionalForeground(const DrawParameter& params)
{
    for(auto it = begin(m_elements); it != end(m_elements); ++it)
        it->get()->drawAdditionalForeground(params);
}

const std::vector<std::unique_ptr<MenuElement>>& MenuPanel::getElements() const
{
    return m_elements;
}