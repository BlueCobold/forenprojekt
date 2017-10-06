
#include "Utility.hpp"

#include "resources/FileReader.hpp"

#include <exception>

#ifdef WINDOWS
#include <Windows.h>
#else
#include "MacHelper.hpp"
#endif

BatteryState getBatteryStateImpl();

namespace utility
{
    /// Specialized Function String 2 String
    template<>
    std::string stringTo(const std::string& str)
    {
        return str;
    }

    b2Vec2 rotate(const b2Vec2& v, float angle)
    {
        return b2Vec2(cos(angle) * v.x + sin(angle) * v.y, -sin(angle) * v.x + cos(angle) * v.y);
    }

    int hexToInt(const std::string& hex)
    {
        std::stringstream ss;
        ss << std::hex << hex;
        int i;
        ss >> i;
        return i;
    }

    sf::Color hexToColor(const std::string& hex)
    {
        sf::Color color;
        if(hex.length() < 8)
            throw std::runtime_error(translateKey("@InvalidColorFormat"));
        color.r = hexToInt(hex.substr(0, 2));
        color.g = hexToInt(hex.substr(2, 2));
        color.b = hexToInt(hex.substr(4, 2));
        color.a = hexToInt(hex.substr(6, 2));
        return color;
    }

    std::string _language;
    void setLanguage(const std::string& language)
    {
        _language = language;
    }

    std::string translateKey(const std::string& key)
    {
        static FileReader File("res/language.dat", true);
        // don't attach language key to strings starting with @
        // don't translate at all if it starts with @@
        if(key.length() >= 2 && key[0] == '@')
        {
            if(key[1] == '@')
                return key.substr(2);
            return File.get(key.substr(1));
        }
        return File.get(_language + "_" + key);
    }

    std::string replace(std::string string, std::string needle, std::string replacement)
    {
        auto pos = string.find(needle);
        if(pos != std::string::npos)
            string.replace(pos, needle.length(), replacement);

        return string;
    }

    std::string replaceAll(std::string string, std::string needle, std::string replacement)
    {
        for(auto pos = string.find(needle); pos != std::string::npos; pos = string.find(needle))
            string.replace(pos, needle.length(), replacement);

        return string;
    }

    std::string replace(std::string string, std::string replacement)
    {
        auto pos = string.find("%");
        if(pos != std::string::npos)
            string.replace(pos, 1, replacement);

        return string;
    }

    sf::View getDefaultView(const sf::RenderTarget& target, const sf::Vector2f viewSize)
    {
        return getDefaultView(target, sf::Vector2u(
            static_cast<unsigned int>(viewSize.x),
            static_cast<unsigned int>(viewSize.y)));
    }

    sf::View getDefaultView(const sf::RenderTarget& target, const sf::Vector2u viewSize)
    {
        auto view = sf::View(
            sf::FloatRect(0, 0,
            static_cast<float>(viewSize.x),
            static_cast<float>(viewSize.y)));
        view.setViewport(sf::FloatRect(0, 0,
            view.getSize().x / target.getSize().x,
            view.getSize().y / target.getSize().y));
        return view;
    }

    std::string floatToPlayTimeString(float playtime)
    {
        unsigned int minute;
        unsigned int second;
        unsigned int milliSecond;
        std::string result = "";

        milliSecond = static_cast<unsigned int>(playtime * 1000);
        second = milliSecond / 1000;
        milliSecond = milliSecond % 1000;
        minute = second / 60;
        second = second % 60;

        if(minute < 10)
            result.append("0");

        result.append(utility::toString(minute));

        if(second < 10)
            result.append(":0");
        else
            result.append(":");

        result.append(utility::toString(second));

        if(milliSecond < 10)
            result.append(":00");
        else if(milliSecond < 100)
            result.append(":0");
        else
            result.append(":");

        result.append(utility::toString(milliSecond));

        return result;
    }

    std::string fileToString(const std::string& file)
    {
        std::string message = "";
        std::string line = "";
        std::ifstream levelFile(file, std::ios_base::in);

        if(levelFile.is_open())
        {
            while(!levelFile.eof())
            {
                std::getline(levelFile, line);
                if(line.length() > 0 && line[line.length() - 1] == '\r')
                    line = line.substr(0, line.length() - 1);
                message.append(line);
            }
        }
        else
            throw std::runtime_error(utility::replace(utility::translateKey("@MissingFile"), file));

        return message;
    }

    std::string getUserName()
    {
#ifdef WINDOWS
        char infoBuffer[128];
        unsigned long bufferCharCount = 128;
        if(GetUserName(infoBuffer, &bufferCharCount))
            return std::string(infoBuffer);
        else
            return std::string("Player");
#elif defined(IOS) || defined(OSX)
        return defaultUserNameApple();
#else
        return "";
#endif
    }

    BatteryState getBatteryState()
	{
		return getBatteryStateImpl();
	}
}
