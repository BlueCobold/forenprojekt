#include "Utility.hpp"
#include "resources/FileReader.hpp"
#include "rendering\transitions\RandomTransition.hpp"

namespace utility
{
	/// Specialized Function String 2 String
	template<>
    std::string stringTo(const std::string& str)
    {
        return str;
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
        color.r = hexToInt(hex.substr(0, 2));
        color.g = hexToInt(hex.substr(2, 2));
        color.b = hexToInt(hex.substr(4, 2));
        color.a = hexToInt(hex.substr(6, 2));
        return color;
    }

    std::string translateKey(std::string key)
    {
        static FileReader File("./res/language.dat");
        return File.get(key);
    }

    std::string replace(std::string string, std::string replacement)
    {
        unsigned int pos = string.find("%");
        if (pos != std::string::npos)
            string.replace(pos,1,replacement);

        return string;
    }

    Transition* getRandomTransition(const sf::Texture* sourceTexture,
                                    const sf::Texture* targetTexture,
                                    const float duration)
    {
        static RandomTransition Transition(21,21);
        return Transition.getRandomTransition(sourceTexture, targetTexture, duration);
    }
}