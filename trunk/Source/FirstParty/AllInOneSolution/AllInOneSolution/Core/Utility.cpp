#include "Utility.hpp"
#include "resources/FileReader.hpp"

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

    sf::View getDefaultView(const sf::RenderTarget& target, const sf::Vector2u viewSize)
    {
        auto view = sf::View(
            sf::FloatRect(0, 0,
            static_cast<float>(viewSize.x),
            static_cast<float>(viewSize.y)));
        view.setViewport(sf::FloatRect(0, 0,
            viewSize.x / static_cast<float>(target.getSize().x),
            viewSize.y / static_cast<float>(target.getSize().y)));
        return view;
    }
}
