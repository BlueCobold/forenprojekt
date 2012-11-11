#include "App.hpp"

App::App(std::string& windowTitle, const unsigned int screenWidth, const unsigned int screenHeight,
    const unsigned int bitsPerPixel, const bool fullscreen)
: m_windowTitle(windowTitle),
  m_screenWidth(screenWidth),
  m_screenHeight(screenHeight),
  m_bitsPerPixel(bitsPerPixel),
  m_fullscreen(fullscreen),
  m_lastTime(0),
  m_currentTime(0)

{
    if(fullscreen)
    {
        m_screen.create(sf::VideoMode(screenWidth, screenHeight, bitsPerPixel), windowTitle, sf::Style::Fullscreen);
        // Disable the cursor
        m_screen.setMouseCursorVisible(false);
    }
    else
        m_screen.create(sf::VideoMode(screenWidth, screenHeight, bitsPerPixel), windowTitle);

    m_font.loadFromFile("visitor.ttf");

    m_fpsText.setFont(m_font);
    m_fpsText.setColor(sf::Color::Yellow);
    m_fpsText.setCharacterSize(30);
    m_fpsText.setPosition(10, 10);
}


App::~App()
{
}

void App::run()
{
    while(m_screen.isOpen())
    {
        update();
        draw();
    }
}

void App::update()
{
    calculateFps();
    m_fpsText.setString(floatToString(m_fps));
   
    handleEvents();
    handleKeyboard();	
}

void App::draw()
{
    m_screen.clear();
    
    m_screen.draw(m_fpsText);

    m_screen.display();
}

void App::handleKeyboard()
{
    // Enter to window or fullscreen mode when Press Return+LAlt
    if((sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)))
    {
        m_fullscreen = !m_fullscreen;
        
        switchDisplayMode();
    }
}
void App::handleEvents()
{
    sf::Event event;
    
    while(m_screen.pollEvent(event))
    {
        // Close the window
        if(event.type == sf::Event::Closed)
            m_screen.close();
    }
}

void App::switchDisplayMode()
{
    if(m_fullscreen)
    {	
        // Switch to fullscreen
        m_screen.create(sf::VideoMode(m_screenWidth, m_screenHeight, m_bitsPerPixel),
        m_windowTitle, sf::Style::Fullscreen);
        // Disable the cursor
        m_screen.setMouseCursorVisible(false);
    }
    else
    {
        // Switch to window mode
        m_screen.create(sf::VideoMode(m_screenWidth, m_screenHeight, m_bitsPerPixel), m_windowTitle);
        // Enable the cursor
        m_screen.setMouseCursorVisible(true);
    }
}

void App::calculateFps()
{
    m_currentTime = clock.getElapsedTime().asSeconds();
    m_fps = 1.f / (m_currentTime - m_lastTime);
    m_lastTime = m_currentTime;
}

std::string App::floatToString(float value)
{
    std::ostringstream out;

    out << value;

    return out.str();
}