#pragma once

#ifndef OPENFILEDIALOGE_HPP
#define OPENFILEDIALOGE_HPP

#include <string>

class OpenFileDialoge
{
public:
    OpenFileDialoge(const char* filter);
    bool openDialoge();
    std::string getFile();

private:
    const char* m_filter;
    std::string m_file;
};

#endif