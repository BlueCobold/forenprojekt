#pragma once

#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

class FileReader
{
public:

    FileReader(const std::string& fileName);

    virtual ~FileReader() {};

    virtual std::string get(const std::string& key);
    
    void reload(const std::string& fileName);

protected:

    std::string eraseOverhang(std::string& data);

    void readFile();

protected:

    std::string m_fileName;
    std::unordered_map<std::string, std::string> m_content;
};

#endif