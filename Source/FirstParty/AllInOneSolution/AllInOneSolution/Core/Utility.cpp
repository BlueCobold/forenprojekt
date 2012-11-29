#include "Utility.hpp"

namespace utility
{
	/// Specialized Function String 2 String
	template<>
    std::string stringTo(const std::string& str)
    {
        return str;
    }
}