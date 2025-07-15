#pragma once
#include <string>
#include <stdint.h>

namespace AnalyticalApproach::Spellcore
{
    class Resource
    {
        uint64_t _uid;
        std::string _path; 
        std::string _name; 

        Resource() = default;
    public:
        void SetPath(const std::string&& path)
        {
            _path  = std::move(path); 
        }

        const std::string& GetPath()
        {
            return _path; 
        }

        virtual void Init() = 0;
    };
}