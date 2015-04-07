#include "OpenFileDialoge.hpp"

OpenFileDialoge::OpenFileDialoge(const char* filter)
{
    m_file[0] = '\0';

    ZeroMemory(&m_openFileName, sizeof(m_openFileName));
    m_openFileName.lStructSize = sizeof(m_openFileName);
    m_openFileName.hwndOwner = nullptr;
    m_openFileName.lpstrFile = m_file;
    m_openFileName.nMaxFile = sizeof(m_file);
    m_openFileName.lpstrFilter = filter;
    m_openFileName.nFilterIndex = 1;
    m_openFileName.lpstrFileTitle = nullptr;
    m_openFileName.nMaxFileTitle = 0;
    m_openFileName.lpstrInitialDir = nullptr;
    m_openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
}

bool OpenFileDialoge::openDialoge()
{
    char currentPath[bufferLength];

    GetCurrentDirectory(bufferLength, currentPath);

    int result = GetOpenFileName(&m_openFileName);

    SetCurrentDirectory(currentPath);

    if(result == 1)
        return true;
    else
        return false;
}

std::string OpenFileDialoge::getFile()
{
    return std::string(m_file);
}