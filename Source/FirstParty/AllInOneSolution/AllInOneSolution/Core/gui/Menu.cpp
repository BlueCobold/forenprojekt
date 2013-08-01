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

    for(auto it = begin(m_subWindow); it != end(m_subWindow); ++it)
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

    for(auto it = begin(m_subWindow); it != end(m_subWindow); ++it)
        (*it)->draw(params);

    for(auto it = begin(m_buttons); it != end(m_buttons); ++it)
        (*it)->drawAdditionalForeground(params);
}

void Menu::drawAdditionalBackground(const DrawParameter& params)
{
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

    for(auto it = begin(m_subWindow); it != end(m_subWindow); ++it)
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

void Menu::createSubWindow(const SubWindowInfo& info)
{
    std::unique_ptr<SubWindow> subWindow(new SubWindow(m_position, info.size, info.virtualPosition, info.position, info.innerHeight, info.menuElements));

    m_subWindow.push_back(std::move(subWindow));
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
    throw std::runtime_error(utility::replace(utility::translateKey("CheckboxId"), utility::toString(id)));
}

Slider& Menu::getSlider(int id)
{
    for(auto it = begin(m_slider); it != end(m_slider); ++it)
    {
        if(it->get()->getId() == id)
            return *it->get();
    }
    throw std::runtime_error(utility::replace(utility::translateKey("sliderID"), utility::toString(id)));
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

void Menu::changeIdleSprite(const int id, const sf::Sprite& sprite)
{
    for(auto button = begin(m_buttons); button != end(m_buttons); ++button)
        if(button->get()->getId() == id)
            button->get()->changeIdleSprite(sprite);
}

void Menu::changeHoverSprite(const int id, const sf::Sprite& sprite)
{
    for(auto button = begin(m_buttons); button != end(m_buttons); ++button)
        if(button->get()->getId() == id)
            button->get()->changeHoverSprite(sprite);
}

void Menu::changePressedSprite(const int id, const sf::Sprite& sprite)
{
    for(auto button = begin(m_buttons); button != end(m_buttons); ++button)
        if(button->get()->getId() == id)
            button->get()->changePressedSprite(sprite);
}
