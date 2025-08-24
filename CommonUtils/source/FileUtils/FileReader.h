#pragma once
#include "FileReader.h"
#include <string>
#include <fstream>
#include <optional>

namespace AnalyticalApproach::FileSystem
{
    class FileReader 
    {
    public:
        FileReader(const std::string &path);

        std::string getContent() ;
        std::string getPath(); 
    private:
        std::string _filePath;
    };
}