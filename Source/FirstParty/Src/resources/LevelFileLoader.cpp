#include "LevelFileLoader.hpp"

std::vector<std::string> LevelFileLoader::parseGrid(const tinyxml2::XMLElement& xml)
{
    std::string data = xml.GetText();

    // Remove first and last line breaks
    data = data.substr(data.find_first_of('\n')+1, data.find_last_of('\n'));

    std::vector<std::string> lines;

    // Get the lines separated without the newline char
    for(std::size_t i = 0; i < data.length(); i = data.find('\n', i) + 1)
        lines.push_back(data.substr(i, data.find('\n', i) - i));

    return std::move(lines);
}
