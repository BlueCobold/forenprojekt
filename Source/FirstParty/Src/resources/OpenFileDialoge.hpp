#pragma once

#ifndef OPENFILEDIALOGE_HPP
#define OPENFILEDIALOGE_HPP

#include <iostream>
#include <Windows.h>

class OpenFileDialoge
{
private:
    static const unsigned long bufferLength = 255;

    OPENFILENAME m_openFileName;
    char m_file[bufferLength];

public:
    OpenFileDialoge(const char* filter);
    bool openDialoge();
    std::string getFile();
};

#endif