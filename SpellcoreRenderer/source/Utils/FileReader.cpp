#include "FileReader.h"
#include <sstream>
#include <fstream>
#include "Logger.h"

namespace AnalyticalApproach::FileSystem
{

    FileReader::FileReader(const std::string &path)
        : _filePath(path) {}

    std::string FileReader::getContent()
    {
        std::ifstream file(_filePath);
        if (!file.is_open())
        {
            std::string errorMsg = "Can not open file at path: " + _filePath; 
            LOG_ERROR(_filePath);
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    std::string FileReader::getPath()
    {
        return _filePath;
    }

}