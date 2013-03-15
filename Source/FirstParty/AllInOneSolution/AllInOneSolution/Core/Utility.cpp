#include "Utility.hpp"
#include "FileReader.hpp"

namespace utility
{
	/// Specialized Function String 2 String
	template<>
    std::string stringTo(const std::string& str)
    {
        return str;
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
}