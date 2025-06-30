#include "FileReader.h"
#include <sstream>
#include <fstream>

namespace AnalyticalApproach::FileSystem
{

    FileReader::FileReader(const std::string &path)
        : _filePath(path) {}

    std::string FileReader::getContent()
    {
        std::ifstream file(_filePath);
        if (!file.is_open())
            return "";

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    std::string FileReader::getPath()
    {
        return _filePath;
    }

}