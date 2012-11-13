#include "App.hpp"
#include "Config.hpp"

int main(int argc, char* argv[])
{
    Config ConfigFile("Config.ini");
	
    App app(ConfigFile);

	app.run();

	return 0;
}
