#pragma once
#include "Editor/Core/AssetLoaders/ObjLoader.h"

namespace AnalyticalApproach::SpellcoreEditor
{
	using namespace Spellcore; 
	class ObjLoaderTest
	{
		ObjLoader* _objLoader;

		std::string CUBE_OBJ_PATH = "/Resources/Models/ObjWavefront/Cube.obj"; 
		std::string CONE_OBJ_PATH = "/Resources/Models/ObjWavefront/Cone.obj"; 

	public: 
		ObjLoaderTest(); 
		~ObjLoaderTest(); 

		void LoadDefaultMeshes(); 

		MeshData* GetCubeMesh(); 
		MeshData* GetConeMesh(); 
	};
}