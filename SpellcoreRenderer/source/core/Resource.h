#pragma once
#include <string> 
namespace AnalyticalApproach::Spellcore
{
    class Resource
    {
        public: 
        virtual bool LoadFromPath(std::string path) = 0; 
    }
}