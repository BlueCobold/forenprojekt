#include "Utility.hpp"
#include "resources/FileReader.hpp"

#include <exception>

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
            throw std::runtime_error(translateKey("InvalidColorFormat"));
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
            // because of rounding/casting-crap in SFML (RenderTarget::getViewport),
            // pretend the view to be larger, so that no error-pixels occur
            static_cast<float>(viewSize.x) + 0.2f,
            static_cast<float>(viewSize.y) + 0.2f));
        view.setViewport(sf::FloatRect(0, 0,
            view.getSize().x / target.getSize().x,
            view.getSize().y / target.getSize().y));
        return view;
    }
}
