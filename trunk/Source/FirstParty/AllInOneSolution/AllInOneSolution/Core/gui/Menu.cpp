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

    for(auto checkbox = begin(m_template.checkboxes); checkbox != end(m_template.checkboxes); ++checkbox)
        createCheckBox(*checkbox);

    for(auto slider = begin(m_template.slider); slider != end(m_template.slider); ++slider)
        createSlider(*slider);

    for(auto label = begin(m_template.labels); label != end(m_template.labels); ++label)
        createLabel(*label);

    for(auto sprite = begin(m_template.sprites); sprite != end(m_template.sprites); ++sprite)
        createSprite(*sprite);
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

    for(auto it = begin(m_checkBoxes); it != end(m_checkBoxes); ++it)
        (*it)->setPosition(m_position);

    for(auto it = begin(m_slider); it != end(m_slider); ++it)
        (*it)->setPosition(m_position);

    for(auto it = begin(m_labels); it != end(m_labels); ++it)
        (*it)->setPosition(m_position);

    for(auto it = begin(m_sprites); it != end(m_sprites); ++it)
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

    for(auto it = begin(m_checkBoxes); it != end(m_checkBoxes); ++it)
        (*it)->draw(params);

    for(auto it = begin(m_slider); it != end(m_slider); ++it)
        (*it)->draw(params);

    for(auto it = begin(m_labels); it != end(m_labels); ++it)
        (*it)->draw(params);

    for(auto it = begin(m_sprites); it != end(m_sprites); ++it)
        (*it)->draw(params);
}

void Menu::update(const sf::RenderWindow& screen)
{
    for(auto it = begin(m_buttons); it != end(m_buttons); ++it)
        (*it)->update(screen);

    for(auto it = begin(m_checkBoxes); it != end(m_checkBoxes); ++it)
        (*it)->update(screen);

    for(auto it = begin(m_slider); it != end(m_slider); ++it)
        (*it)->update(screen);

    for(auto it = begin(m_sprites); it != end(m_sprites); ++it)
        (*it)->update(screen);
}

void Menu::createButton(const ButtonInfo& info)
{
    std::unique_ptr<Button> button(new Button(info.id, info.style, info.sound, m_position, info.position));

    button->registerOnPressed([this](const Button& sender)
    {
        if(m_clickCallback != nullptr)
            m_clickCallback(sender);
    });

    m_buttons.push_back(std::move(button));
}

void Menu::createCheckBox(const CheckBoxInfo& info)
{
    std::unique_ptr<CheckBox> checkbox(new CheckBox(info.id, info.style, m_position, info.position));

    m_checkBoxes.push_back(std::move(checkbox));
}

void Menu::createSlider(const SliderInfo& info)
{
    std::unique_ptr<Slider> slider(new Slider(info.id, info.style, m_position, info.position));

    m_slider.push_back(std::move(slider));
}

void Menu::createLabel(const LineLabel& info)
{
    std::unique_ptr<LineLabel> label(new LineLabel(info));

    m_labels.push_back(std::move(label));
}

void Menu::createSprite(const MenuSprite& info)
{
    std::unique_ptr<MenuSprite> sprite(new MenuSprite(info));

    m_sprites.push_back(std::move(sprite));
}
void Menu::registerOnClick(std::function<void(const Button& sender)> callback)
{
    m_clickCallback = callback;
}

CheckBox& Menu::getCheckboxes(int id)
{
    for(auto it = begin(m_checkBoxes); it != end(m_checkBoxes); ++it)
    {
        if(it->get()->getId() == id)
            return *it->get();
    }
    throw std::runtime_error(utility::replace(utility::translateKey("sliderID"), utility::toString(id)));
}

Slider& Menu::getSlider(int id)
{
    for(auto it = begin(m_slider); it != end(m_slider); ++it)
    {
        if(it->get()->getId() == id)
            return *it->get();
    }
    throw std::runtime_error(utility::replace(utility::translateKey("CheckboxId"), utility::toString(id)));
}
sf::RenderWindow& Menu::getRenderWindow()
{
    return m_screen;
}

LineLabel& Menu::getLabel(int id)
{
    for(auto it = begin(m_labels); it != end(m_labels); ++it)
    {
        if(it->get()->getId() == id)
            return *it->get();
    }
    throw std::runtime_error(utility::replace(utility::translateKey("LineLabelId"), utility::toString(id)));
}

MenuSprite& Menu::getSprite(int id)
{
    for(auto it = begin(m_sprites); it != end(m_sprites); ++it)
    {
        if(it->get()->getId() == id)
            return *it->get();
    }
    throw std::runtime_error(utility::replace(utility::translateKey("MenuSpriteId"), utility::toString(id)));
}