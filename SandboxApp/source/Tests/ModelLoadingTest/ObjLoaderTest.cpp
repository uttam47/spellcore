#include "ObjLoaderTest.h" 
#include "Editor/Core/ResourceManager.h"

namespace AnalyticalApproach::SpellcoreEditor
{
	using namespace Spellcore; 
	ObjLoaderTest::ObjLoaderTest()
	{
		CUBE_OBJ_PATH = ResourceManager::GetExecutionDir() + CUBE_OBJ_PATH;
		CONE_OBJ_PATH = ResourceManager::GetExecutionDir() + CONE_OBJ_PATH;
		_objLoader = new ObjLoader(); 
	}

	void ObjLoaderTest::LoadDefaultMeshes()
	{
		cubeMesh = _objLoader->Load(CUBE_OBJ_PATH, std::optional<bool>(true), VertexPacking::Interleaved);
		coneMesh = _objLoader->Load(CONE_OBJ_PATH, std::optional<bool>(true), VertexPacking::Interleaved);
	}

	MeshData& ObjLoaderTest::GetConeMesh()
	{
		return coneMesh; 
	}

	MeshData& ObjLoaderTest::GetCubeMesh()
	{
		return cubeMesh;
	}

	ObjLoaderTest::~ObjLoaderTest()
	{
		delete _objLoader; 
		_objLoader = nullptr; 
	}
}