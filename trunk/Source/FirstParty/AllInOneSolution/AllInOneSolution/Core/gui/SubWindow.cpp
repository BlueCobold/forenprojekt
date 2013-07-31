#include "SubWindow.hpp"
#include "Button.hpp"
#include "../rendering/DrawParameter.hpp"
#include "../Input.hpp"
#include "../gui/Menu.hpp"

SubWindow::SubWindow(const sf::Vector2f& position,
                     const sf::Vector2f& size,
                     const sf::Vector2f& innerPosition,
                     const sf::Vector2f& offset,
                     const int innerHeight,
                     const MenuElements& elements) :
    m_position(position),
    m_size(size),
    m_offset(offset),
    m_innerHeight(innerHeight),
    m_innerPosition(innerPosition),
    m_center(sf::Vector2f(0, 0)),
    m_startValue(0),
    m_endValue(0)
{
    m_windowRect.setPosition(m_position.x + m_offset.x,
                             m_position.y + m_offset.y);
    m_windowRect.setSize(size);
    m_windowRect.setFillColor(sf::Color(255,128,128,80));

    m_sliderRect.setPosition(m_position.x + m_offset.x + m_size.x,
                             m_position.y + m_offset.y);
    m_sliderRect.setSize(sf::Vector2f(14, m_size.y));
    m_sliderRect.setFillColor(sf::Color(128,255,128,128));

    m_positionRect.setPosition(m_position.x + m_offset.x + m_size.x + 2,
                               m_position.y + m_offset.y);

    float positionSize = 1.f - (m_innerHeight - m_size.y) / m_innerHeight;
    if(positionSize > 1.f)
        positionSize = 1.f;

    m_positionRect.setSize(sf::Vector2f(10, m_size.y * positionSize));
    m_positionRect.setFillColor(sf::Color(128,128,255,128));

    m_center.x = m_innerPosition.x + m_size.x / 2.f;
    m_center.y = m_innerPosition.y + m_size.y / 2.f;

    for(auto label = begin(elements.labels); label != end(elements.labels); ++label)
        createLabel(*label);

    for(auto sprite = begin(elements.sprites); sprite != end(elements.sprites); ++sprite)
        createSprite(*sprite);

    for(auto checkbox = begin(elements.checkboxes); checkbox != end(elements.checkboxes); ++checkbox)
        createCheckBox(*checkbox);

    for(auto slider = begin(elements.slider); slider != end(elements.slider); ++slider)
        createSlider(*slider);

    for(auto button = begin(elements.buttons); button != end(elements.buttons); ++button)
        createButton(*button);
}

void SubWindow::on(const DrawParameter& params)
{
    m_orginalView = params.getTarget().getView();
    sf::View windowView;
    sf::FloatRect orginalScreenRect = params.getScreenRect();
    sf::FloatRect windowViewport;
    windowViewport.left = (m_position.x + m_offset.x) / orginalScreenRect.width;
    windowViewport.top = (m_position.y + m_offset.y) / orginalScreenRect.height;
    windowViewport.width = m_size.x / orginalScreenRect.width;
    windowViewport.height = m_size.y / orginalScreenRect.height;
    windowView.setViewport(windowViewport);
    windowView.setCenter(m_center);
    windowView.setSize(m_size);
    params.getTarget().setView(windowView);
}
void SubWindow::off(const DrawParameter& params)
{
    params.getTarget().setView(m_orginalView);
}
void SubWindow::draw(const DrawParameter& params)
{
    params.getTarget().draw(m_windowRect);
    params.getTarget().draw(m_sliderRect);
    params.getTarget().draw(m_positionRect);

    on(params);
    for(auto it = begin(m_labels); it != end(m_labels); ++it)
        (*it)->draw(params);
    for(auto it = begin(m_buttons); it != end(m_buttons); ++it)
        (*it)->draw(params);
    for(auto it = begin(m_sprites); it != end(m_sprites); ++it)
        (*it)->draw(params);
    for(auto it = begin(m_checkBoxes); it != end(m_checkBoxes); ++it)
        (*it)->draw(params);
    for(auto it = begin(m_slider); it != end(m_slider); ++it)
        (*it)->draw(params);
    off(params);    
}
void SubWindow::update(const sf::RenderWindow& screen)
{
    sf::IntRect mouseRect(static_cast<sf::Vector2i>(m_windowRect.getPosition()), static_cast<sf::Vector2i>(m_windowRect.getSize()));
    sf::IntRect sliderRect(static_cast<sf::Vector2i>(m_positionRect.getPosition()), static_cast<sf::Vector2i>(m_positionRect.getSize()));

    if(mouseRect.contains(sf::Mouse::getPosition(screen)) && utility::Mouse.isWheelMovedDown())
    {
        if((m_center.y + percentToWindowPixels()) < (m_innerPosition.y + m_innerHeight - m_size.y / 2.f))
            m_center.y += percentToWindowPixels();
        else
            m_center.y = m_innerPosition.y + m_innerHeight - m_size.y / 2.f;
    }
    else if(mouseRect.contains(sf::Mouse::getPosition(screen)) && utility::Mouse.isWheelMovedUp())
    {
        if((m_center.y - percentToWindowPixels()) > (m_innerPosition.y + m_size.y / 2.f))
            m_center.y -= percentToWindowPixels();
        else
            m_center.y = m_innerPosition.y + m_size.y / 2.f;
    }
    
    if(sliderRect.contains(sf::Mouse::getPosition(screen)) && utility::Mouse.leftButtonDown())
        m_startValue = sf::Mouse::getPosition().y;
    else if(utility::Mouse.leftButtonPressed())
    {
        m_endValue = sf::Mouse::getPosition().y - m_startValue;
        m_center.y += sliderPixelToWindowPixel(m_endValue);
        if(m_center.y < (m_innerPosition.y + m_size.y / 2.f))
            m_center.y = m_innerPosition.y + m_size.y / 2.f;
        else if(m_center.y > (m_innerPosition.y + m_innerHeight - m_size.y / 2.f))
            m_center.y = m_innerPosition.y + m_innerHeight - m_size.y / 2.f;
    }
    setSliderPosition();
}
void SubWindow::setOffset(const sf::Vector2f& offset)
{
    m_offset = offset;

    m_windowRect.setPosition(m_position.x + m_offset.x,
                             m_position.y + m_offset.y);
    m_sliderRect.setPosition(m_position.x + m_offset.x + m_size.x,
                             m_position.y + m_offset.y);
    m_positionRect.setPosition(m_position.x + m_offset.x + m_size.x + 2,
                               m_position.y + m_offset.y);
}

void SubWindow::setPosition(const sf::Vector2f& position)
{
    m_position = position;

    m_windowRect.setPosition(m_position.x + m_offset.x,
                             m_position.y + m_offset.y);
    m_sliderRect.setPosition(m_position.x + m_offset.x + m_size.x,
                             m_position.y + m_offset.y);
    m_positionRect.setPosition(m_position.x + m_offset.x + m_size.x + 2,
                               m_position.y + m_offset.y);
}
void SubWindow::setInnerPosition(const sf::Vector2f& position)
{
    m_innerPosition = position;

    for(auto it = begin(m_buttons); it != end(m_buttons); ++it)
        (*it)->setPosition(m_innerPosition);

    for(auto it = begin(m_checkBoxes); it != end(m_checkBoxes); ++it)
        (*it)->setPosition(m_innerPosition);

    for(auto it = begin(m_slider); it != end(m_slider); ++it)
        (*it)->setPosition(m_innerPosition);

    for(auto it = begin(m_labels); it != end(m_labels); ++it)
        (*it)->setPosition(m_innerPosition);

    for(auto it = begin(m_sprites); it != end(m_sprites); ++it)
        (*it)->setPosition(m_innerPosition);
}

void SubWindow::createCheckBox(const CheckBoxInfo& info)
{
    std::unique_ptr<CheckBox> checkbox(new CheckBox(info.id, info.style, m_position, info.position));

    m_checkBoxes.push_back(std::move(checkbox));
}

void SubWindow::createSlider(const SliderInfo& info)
{
    std::unique_ptr<Slider> slider(new Slider(info.id, info.style, m_position, info.position));

    m_slider.push_back(std::move(slider));
}

void SubWindow::createButton(const ButtonInfo& info)
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

void SubWindow::createLabel(const LineLabel& info)
{
    std::unique_ptr<LineLabel> label(new LineLabel(info));

    m_labels.push_back(std::move(label));
}

void SubWindow::createSprite(const MenuSprite& info)
{
    std::unique_ptr<MenuSprite> sprite(new MenuSprite(info));

    m_sprites.push_back(std::move(sprite));
}

float SubWindow::percentToWindowPixels(float percent)
{
    return static_cast<float>(m_innerHeight - m_size.y) / 100.f * percent;
}
float SubWindow::sliderPixelToWindowPixel(int pixel)
{
    float pixelPercent = static_cast<float>(pixel / static_cast<float>(m_sliderRect.getSize().y - m_positionRect.getSize().y));
    return percentToWindowPixels(pixelPercent);
}
void SubWindow::setSliderPosition()
{
    float currentPercent = (m_innerPosition.y + m_size.y / 2.f - m_center.y) / (m_innerHeight - m_size.y) ;
    int currentPosition = static_cast<int>((m_sliderRect.getSize().y - m_positionRect.getSize().y) * -currentPercent);
    m_positionRect.setPosition(m_position.x + m_offset.x + m_size.x + 2,
                                m_position.y + m_offset.y + currentPosition);
}