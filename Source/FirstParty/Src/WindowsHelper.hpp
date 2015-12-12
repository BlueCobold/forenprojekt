#pragma once

#ifndef WINDOWS_HELPER_HPP
#define WINDOWS_HELPER_HPP

#ifdef WINDOWS

#include <Windows.h>
#include <string>

bool pickFileWin(std::string& path, const char* filter)
{
    const auto bufferLength = 1024;
    char file[bufferLength];
    file[0] = '\0';
    
    OPENFILENAME openFileName;
    ZeroMemory(&openFileName, sizeof(openFileName));
    openFileName.lStructSize = sizeof(openFileName);
    openFileName.hwndOwner = nullptr;
    openFileName.lpstrFile = file;
    openFileName.nMaxFile = sizeof(file);
    openFileName.lpstrFilter = filter;
    openFileName.nFilterIndex = 1;
    openFileName.lpstrFileTitle = nullptr;
    openFileName.nMaxFileTitle = 0;
    openFileName.lpstrInitialDir = nullptr;
    openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    
    char currentPath[bufferLength];
    GetCurrentDirectory(bufferLength, currentPath);
    int result = GetOpenFileName(&openFileName);
    
    SetCurrentDirectory(currentPath);
    if(result == 1)
        path = std::string(file);
        
    return result == 1;
}

#endif

#endif // WINDOWS_HELPER_HPP
