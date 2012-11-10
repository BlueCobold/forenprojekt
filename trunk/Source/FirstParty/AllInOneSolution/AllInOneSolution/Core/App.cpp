#include "App.h"


App::App(std::string& windowTitle, const unsigned int& screenWidth, const unsigned int& screenHeight,
	const unsigned int& bitsPerPixel, const bool& fullscreen) 
: m_windowTitle(windowTitle),
  m_screenWidth(screenWidth),
  m_screenHeight(screenHeight),
  m_bitsPerPixel(bitsPerPixel),
  m_fullscreen(fullscreen)
{
	if(fullscreen)
		m_window.create(sf::VideoMode(screenWidth, screenHeight, bitsPerPixel), windowTitle, sf::Style::Fullscreen);
	else
		m_window.create(sf::VideoMode(screenWidth, screenHeight, bitsPerPixel), windowTitle);
}


App::~App()
{
}

void App::run()
{
	while(m_window.isOpen())
	{
		update();
		draw();
	}
}

void App::update()
{
	handleEvents();
	handleKeyboard();	
}

void App::draw()
{
	m_window.clear();

	m_window.display();
}

void App::handleKeyboard()
{
	// Enter to window or fullscreen mode when Press Return+LAlt
	if((sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)))
	{
		m_fullscreen = !m_fullscreen;

		if(m_fullscreen)
			m_window.create(sf::VideoMode(m_screenWidth, m_screenHeight, m_bitsPerPixel),
			m_windowTitle, sf::Style::Fullscreen);
		else
			m_window.create(sf::VideoMode(m_screenWidth, m_screenHeight, m_bitsPerPixel), m_windowTitle);
	}
}
void App::handleEvents()
{
	sf::Event event;
	
	while(m_window.pollEvent(event))
	{
		// Close the window
		if(event.type == sf::Event::Closed)
			m_window.close();
	}
}
