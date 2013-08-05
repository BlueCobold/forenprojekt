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
        m_template(menuTemplate)
{
    m_template.background.setPosition(m_position);

    m_size = sf::Vector2i(m_template.background.getTextureRect().width, m_template.background.getTextureRect().height);

    for(auto button = begin(m_template.menuElements.buttons); button != end(m_template.menuElements.buttons); ++button)
        createButton(*button);

    for(auto checkbox = begin(m_template.menuElements.checkboxes); checkbox != end(m_template.menuElements.checkboxes); ++checkbox)
        createCheckBox(*checkbox);

    for(auto slider = begin(m_template.menuElements.slider); slider != end(m_template.menuElements.slider); ++slider)
        createSlider(*slider);

    for(auto label = begin(m_template.menuElements.labels); label != end(m_template.menuElements.labels); ++label)
        createLabel(*label);

    for(auto sprite = begin(m_template.menuElements.sprites); sprite != end(m_template.menuElements.sprites); ++sprite)
        createSprite(*sprite);

    for(auto subWindow = begin(m_template.subWindow); subWindow != end(m_template.subWindow); ++subWindow)
        createSubWindow(*subWindow);

    setCorrelation();
    std::sort(m_elements.begin(), m_elements.end(), 
        [](const std::unique_ptr<MenuElement>& a, const std::unique_ptr<MenuElement>& b) -> bool
    { 
        return a->getId() < b->getId(); 
    });
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
    
    for(auto it = begin(m_elements); it != end(m_elements); ++it)
        (*it)->setPosition(m_position);
}

const sf::Vector2f& Menu::getPosition() const
{
    return m_position;
}

void Menu::draw(const DrawParameter& params)
{
    params.getTarget().draw(m_template.background);
    drawAdditionalBackground(params);
    
    for(auto it = begin(m_elements); it != end(m_elements); ++it)
        (*it)->draw(params);
}

void Menu::drawAdditionalBackground(const DrawParameter& params)
{
}

void Menu::update(const sf::RenderWindow& screen)
{
    for(auto it = begin(m_elements); it != end(m_elements); ++it)
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

    button->setToolTip(info.toolTip);
    m_elements.push_back(std::move(button));
}

void Menu::createCheckBox(const CheckBoxInfo& info)
{
    std::unique_ptr<CheckBox> checkbox(new CheckBox(info.id, info.style, m_position, info.position));
    m_elements.push_back(std::move(checkbox));
}

void Menu::createSlider(const SliderInfo& info)
{
    std::unique_ptr<Slider> slider(new Slider(info.id, info.style, m_position, info.position));
    m_elements.push_back(std::move(slider));
}

void Menu::createLabel(const LineLabel& info)
{
    std::unique_ptr<LineLabel> label(new LineLabel(info));
    m_elements.push_back(std::move(label));
}

void Menu::createSprite(const MenuSprite& info)
{
    std::unique_ptr<MenuSprite> sprite(new MenuSprite(info));
    m_elements.push_back(std::move(sprite));
}

void Menu::createSubWindow(const SubWindowInfo& info)
{
    std::unique_ptr<SubWindow> subWindow(new SubWindow(m_position, info.size, info.virtualPosition, info.position, info.innerHeight, info.menuElements));
    m_elements.push_back(std::move(subWindow));
}

void Menu::registerOnClick(std::function<void(const Button& sender)> callback)
{
    m_clickCallback = callback;
}

CheckBox& Menu::getCheckboxes(int id)
{
    for(auto it = begin(m_elements); it != end(m_elements); ++it)
    {
        if(it->get()->getType() == MenuElementType::CheckBox && it->get()->getId() == id)
            return *dynamic_cast<CheckBox*>(it->get());
    }
    throw std::runtime_error(utility::replace(utility::translateKey("CheckboxId"), utility::toString(id)));
}

Slider& Menu::getSlider(int id)
{
    for(auto it = begin(m_elements); it != end(m_elements); ++it)
    {
        if(it->get()->getType() == MenuElementType::Slider && it->get()->getId() == id)
            return *dynamic_cast<Slider*>(it->get());
    }
    throw std::runtime_error(utility::replace(utility::translateKey("sliderID"), utility::toString(id)));
}

sf::RenderWindow& Menu::getRenderWindow()
{
    return m_screen;
}

LineLabel& Menu::getLabel(int id)
{
    for(auto it = begin(m_elements); it != end(m_elements); ++it)
    {
        if(it->get()->getType() == MenuElementType::Label && it->get()->getId() == id)
            return *dynamic_cast<LineLabel*>(it->get());
    }
    throw std::runtime_error(utility::replace(utility::translateKey("LineLabelId"), utility::toString(id)));
}

MenuSprite& Menu::getSprite(int id)
{
    for(auto it = begin(m_elements); it != end(m_elements); ++it)
    {
        if(it->get()->getType() == MenuElementType::Image && it->get()->getId() == id)
            return *dynamic_cast<MenuSprite*>(it->get());
    }
    throw std::runtime_error(utility::replace(utility::translateKey("MenuSpriteId"), utility::toString(id)));
}

Button& Menu::getButton(int id)
{
    for(auto it = begin(m_elements); it != end(m_elements); ++it)
    {
        if(it->get()->getType() == MenuElementType::Button && it->get()->getId() == id)
            return *dynamic_cast<Button*>(it->get());
    }
    throw std::runtime_error(utility::replace(utility::translateKey("ButtonId"), utility::toString(id)));
}

void Menu::changeIdleSprite(const int id, const sf::Sprite& sprite)
{
    for(auto it = begin(m_elements); it != end(m_elements); ++it)
        if((*it)->getType() == MenuElementType::Button && (*it)->getId() == id)
            dynamic_cast<Button*>(it->get())->changeIdleSprite(sprite);
}

void Menu::changeHoverSprite(const int id, const sf::Sprite& sprite)
{
    for(auto it = begin(m_elements); it != end(m_elements); ++it)
        if((*it)->getType() == MenuElementType::Button && (*it)->getId() == id)
            dynamic_cast<Button*>(it->get())->changeHoverSprite(sprite);
}

void Menu::changePressedSprite(const int id, const sf::Sprite& sprite)
{
    for(auto it = begin(m_elements); it != end(m_elements); ++it)
        if((*it)->getType() == MenuElementType::Button && (*it)->getId() == id)
            dynamic_cast<Button*>(it->get())->changePressedSprite(sprite);
}

void Menu::setCorrelation()
{
    for(auto s = begin(m_elements); s != end(m_elements); ++s)
    {
        if ((*s)->getType() != MenuElementType::Image)
            continue;
        auto sprite = dynamic_cast<MenuSprite*>(s->get());
        auto id = sprite->getVisibleWhenId();
        if(id == -1 || sprite->getId() == id)
            continue;

        for(auto it = begin(m_elements); it != end(m_elements); ++it)
            if((*it)->getId() == id)
                sprite->setVisibleWhenSubject(it->get());
    }
}
