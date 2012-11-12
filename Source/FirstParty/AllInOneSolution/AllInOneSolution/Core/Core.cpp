#include "App.hpp"
#include "ConfigFileLoader.hpp"

int main(int argc, char* argv[])
{
    ConfigFileLoader ConfigFile("Config.ini");
	
    App app(ConfigFile);

	app.run();

	return 0;
}
