#include "ResourceManager.h"

namespace AnalyticalApproach::Spellcore
{
    
    std::string ResourceManager::EXECUTABLE_DIR; 

        const std::string& ResourceManager::GetExecutionDir()
    {
        return EXECUTABLE_DIR; 
    }

    ResourceManager::ResourceManager(const std::string& executionDirPath)
		{
            EXECUTABLE_DIR = executionDirPath; 
		}
}