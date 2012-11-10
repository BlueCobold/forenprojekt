#include <iostream>
#include <fstream>

/// This Function should be a part of Windowclass
bool LoadConfigFile(std::string FileName);

int main(int argc, char* argv[])
{
	const std::string CONFIGFILE = "Config.ini";
	bool Succesfull = LoadConfigFile(CONFIGFILE);
	return 0;
}

bool LoadConfigFile(std::string FileName)
{
	/// Placeholder for Members of the Windowclass
    int ScreenSizeX = 0;
    int ScreenSizeY = 0;
    bool IsFullScreen = false;

    std::ifstream ConfigFile;
    ConfigFile.open(FileName, std::ios_base::out);
    if(ConfigFile)
    {
        if(ConfigFile >> ScreenSizeX >> ScreenSizeY >> IsFullScreen)
        {
            ConfigFile.close();
            return true;
        }   
    }
    return false;
}