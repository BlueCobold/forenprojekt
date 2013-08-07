#include "FileReader.hpp"

FileReader::FileReader(const std::string& fileName) :
    m_fileName(fileName)
{
    readFile();
}

std::string FileReader::eraseOverhang(std::string& data)
{
    while(!data.find(" "))             /// Find and erase leading spaces
        data.erase(0, 1);

    unsigned int pos = data.find_last_of(" "); 
    
    while(pos == data.length() - 1)    /// Find and erase spaces after data
    {
        data.erase(pos,pos);
        pos = data.find_last_of(" ");
    }
  
    return data;
}

void FileReader::reload(const std::string& fileName)
{
    m_fileName = fileName;
    
    readFile();
}

void FileReader::set(const std::string& key, const std::string& value)
{
    m_content[key] = value;
}

void FileReader::readFile()
{
    // Open file
    std::ifstream configFile(m_fileName, std::ios_base::in);

    if(configFile.is_open())
    {
        // Reset variables
        std::string line = "";
        std::string key = "";
        std::string value = "";
        unsigned int pos = 0;

        while(!configFile.eof())
        {
            std::getline(configFile, line);

            pos = line.find('=');

            // Does not contain a =
            if(pos == std::string::npos)
                continue;

            key = line.substr(0, pos);
            eraseOverhang(key);
            value = line.substr(pos+1);
            eraseOverhang(value);

            if(!key.empty() && !value.empty())
                m_content.insert(std::make_pair<std::string&, std::string&>(key, value));
        } 
    } 
    else
        // cannot use translate here, because translate may need this file which cannot be loaded!
        throw std::runtime_error(std::string("File missing: ") + m_fileName);
}

std::string FileReader::get(const std::string& key)
{
    // Default return value
    std::string output = std::string("Key not found: ") + key;

    auto it = m_content.find(key);

    if(it != end(m_content))
        output = it->second;

    return output;
}

std::string FileReader::getFileName()
{
    return m_fileName;
}

std::unordered_map<std::string, std::string>::const_iterator FileReader::beginIterator()
{
    return m_content.cbegin();
}

std::unordered_map<std::string, std::string>::const_iterator FileReader::endIterator()
{
    return m_content.cend();
}

const bool FileReader::keyExists(const std::string& key) const
{
    for(auto it = begin(m_content); it != end(m_content); ++it)
    {
        if(it->first == key)
            return true;
    }
    return false;
}