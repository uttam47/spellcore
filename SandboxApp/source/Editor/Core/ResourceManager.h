#pragma once
#include <string> 
namespace AnalyticalApproach::Spellcore
{
	//TODO: Questions to consider?
	//1. Does the ResourceManager consider the full reponsibility of the resource it loads, or it is just a utility to load their interal data? Which would be better?
	//2. Does it need be abstract, and can be scaled into multiple types of resource managers?
	//3. When we say Resource do we only mean Asset Resource or it also concern the Computational resources such as Memory, Thread, or Jobs?
	class ResourceManager
	{
		static std::string EXECUTABLE_DIR;


	public:

		static const std::string& GetExecutionDir();

		ResourceManager(const std::string& executionDirPath);
	};
}