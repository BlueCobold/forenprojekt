#include "OpenFileDialoge.hpp"

#ifdef WINDOWS
#include "../WindowsHelper.hpp"
#elif defined(OSX)
#include "../MacHelper.hpp"
#endif

OpenFileDialoge::OpenFileDialoge(const char* filter) :
    m_filter(filter)
{
}

bool OpenFileDialoge::openDialoge()
{
#ifdef WINDOWS
    return pickFileWin(m_file, m_filter);
#elif defined(OSX)
    return pickFileMac(m_file);
#endif
}

std::string OpenFileDialoge::getFile()
{
    return m_file;
}