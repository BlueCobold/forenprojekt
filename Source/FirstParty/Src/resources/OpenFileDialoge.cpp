#include "OpenFileDialoge.hpp"

#ifdef WINDOWS
#include "../WindowsHelper.hpp"
#endif

OpenFileDialoge::OpenFileDialoge(const std::string& filter) :
    m_filter(filter)
{
}

bool OpenFileDialoge::openDialoge()
{
#ifdef WINDOWS
    return pickFileWin(m_file, m_filter);
#endif
}

std::string OpenFileDialoge::getFile()
{
    return m_file;
}