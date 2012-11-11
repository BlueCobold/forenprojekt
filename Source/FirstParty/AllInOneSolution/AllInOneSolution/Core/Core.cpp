#include <string>
#include "App.hpp"
#include "ConfigFileLoader.hpp"

int main(int argc, char* argv[])
{
    ConfigFileLoader ConfigFile("Config.ini");
	/// Create the Application
    App app(ConfigFile.getString("WindowName"),
            ConfigFile.getInt("ResolutionX"), 
            ConfigFile.getInt("ResolutionY"), 
            ConfigFile.getInt("BitsPerPixel"),
            ConfigFile.getBool("IsFullScreen"));

	app.run();

	return 0;
}
