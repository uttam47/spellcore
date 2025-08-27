#include "ObjLoaderTest.h" 
#include "Editor/Core/ResourceManager.h"
#include "core/MeshData.h"

namespace AnalyticalApproach::SpellcoreEditor
{
	using namespace Spellcore; 
	ObjLoaderTest::ObjLoaderTest()
	{
		CUBE_OBJ_PATH = ResourceManager::GetExecutionDir() + CUBE_OBJ_PATH;
		CONE_OBJ_PATH = ResourceManager::GetExecutionDir() + CONE_OBJ_PATH;
		_objLoader = new ObjLoader(); 
	}

	MeshData* ObjLoaderTest::GetConeMesh()
	{
		return _objLoader->Load(CONE_OBJ_PATH, std::optional<bool>(true), VertexPacking::Interleaved);
	}

	MeshData* ObjLoaderTest::GetCubeMesh()
	{
		return _objLoader->Load(CUBE_OBJ_PATH, std::optional<bool>(true), VertexPacking::Interleaved);
	}

	ObjLoaderTest::~ObjLoaderTest()
	{
		delete _objLoader; 
		_objLoader = nullptr; 
	}
}