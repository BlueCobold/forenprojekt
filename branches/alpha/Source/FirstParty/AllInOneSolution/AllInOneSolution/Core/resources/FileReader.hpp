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

    std::string getFileName();
    virtual std::string get(const std::string& key);
    std::unordered_map<std::string, std::string>::const_iterator beginIterator();
    std::unordered_map<std::string, std::string>::const_iterator endIterator();

    void reload(const std::string& fileName);
    const bool keyExists(const std::string& key) const;

protected:

    void set(const std::string& key, const std::string& value);

private:

    std::string eraseOverhang(std::string& data);
    void readFile();

    std::string m_fileName;
    std::unordered_map<std::string, std::string> m_content;
};

#endif
