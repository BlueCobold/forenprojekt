#pragma once

#ifndef OPENFILEDIALOGE_HPP
#define OPENFILEDIALOGE_HPP

#include <string>

class OpenFileDialoge
{
private:
    std::string m_filter;
    std::string m_file;

public:
    OpenFileDialoge(const std::string& filter);
    bool openDialoge();
    std::string getFile();
};

#endif