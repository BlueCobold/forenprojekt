#pragma once

#include <SFML\Graphics.hpp>
#include <string>

/// This class will be used to handle the window or fullscreen
/// and to handle the rendering
class App
{
	public:
	
	App(std::string& windowTitle, const unsigned int& screenWidth, 
		const unsigned int& screenHeight, const unsigned int& bitsPerPixel, const bool& fullscreen);
	~App();

	void run();
	
	private:
	
	void update();
	void draw();

	void handleKeyboard(); /// Will be used to handle the keyboard input
	void handleEvents(); /// Will be used to handle events from the window

	private:

	sf::RenderWindow m_window;

	std::string m_windowTitle;
	unsigned int m_screenWidth, m_screenHeight, m_bitsPerPixel;
	bool m_fullscreen;

};

