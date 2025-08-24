#include "Editor/Core/AssetLoaders/ObjLoader.h"

namespace AnalyticalApproach::SpellcoreEditor
{
	class ObjLoaderTest
	{
		ObjLoader* _objLoader;

		std::string CUBE_OBJ_PATH = ""; 
		std::string CONE_OBJ_PATH = ""; 

		LoadedMesh cubeMesh; 
		LoadedMesh coneMesh; 

	public: 
		ObjLoaderTest(); 

		void LoadDefautls(); 

		LoadedMesh& GetCubeMesh(); 
		LoadedMesh& GetConeMesh(); 
	};
}