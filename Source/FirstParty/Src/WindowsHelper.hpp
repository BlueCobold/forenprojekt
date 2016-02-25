#pragma once

#ifndef WINDOWS_HELPER_HPP
#define WINDOWS_HELPER_HPP

#ifdef WINDOWS

#include "BatteryState.hpp"

#include <Windows.h>
#include <string>

BatteryState getBatteryStateImpl()
{
    BatteryState state;
    SYSTEM_POWER_STATUS status;
    ZeroMemory(&status, sizeof(status));
    if(!GetSystemPowerStatus(&status))
        return state;
    state.percent = status.BatteryLifePercent / 100.f;
    if(status.BatteryFlag == 255)
        state.state = BatteryState::Unknown;
    else if((status.BatteryFlag & 8) != 0 || status.ACLineStatus == 1)
        state.state = BatteryState::Charging;
    else
        state.state = BatteryState::Unplugged;
    return state;
}

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
