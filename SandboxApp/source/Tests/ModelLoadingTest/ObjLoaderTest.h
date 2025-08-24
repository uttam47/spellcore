#include "Editor/Core/AssetLoaders/ObjLoader.h"

namespace AnalyticalApproach::SpellcoreEditor
{
	class ObjLoaderTest
	{
		ObjLoader* _objLoader;

		std::string CUBE_OBJ_PATH = "/Resources/Models/ObjWavefront/Cube.obj"; 
		std::string CONE_OBJ_PATH = "/Resources/Models/ObjWavefront/Cone.obj"; 

		MeshData cubeMesh; 
		MeshData coneMesh; 

	public: 
		ObjLoaderTest(); 
		~ObjLoaderTest(); 

		void LoadDefaultMeshes(); 

		MeshData& GetCubeMesh(); 
		MeshData& GetConeMesh(); 
	};
}