#include <filesystem>
#include "SandboxApp.h"
#include "core/RenderingSurfaceCreateInfo.h"
#include "core/MeshData.h"
#include "core/Mesh.h"

namespace AnalyticalApproach::SpellcoreEditor
{
	using namespace WindowSystem;
	using namespace EventSystem;
	using namespace Spellcore; 

	SandboxApp::SandboxApp(std::vector<std::string> appParameters)
	{
		_resourceManager = new ResourceManager(std::filesystem::path(appParameters[0]).parent_path().string());
		InitAppWindow(); 
		InitRenderer();		
		TestingHelper(); 
	}

	void SandboxApp::TestingHelper()
	{
		//Imgui Test object
		_imguiTest.Init(_windowHandle.glfwWindow);

		_objLoaderTest = new ObjLoaderTest(); 

		_testMesh = new Mesh(); 
		MeshData* meshData = _objLoaderTest->GetCubeMesh(); 
		Submesh* submesh = new Submesh(meshData);
		_testMesh->submeshes.push_back(submesh);

	}

	void SandboxApp::InitAppWindow()
	{
		_windowEventChannel = EventManager::GetInstance().GetEventChannel<WindowEventChannel>();
		_windowHandle = _windowSystem.CreateAppWindow(1280, 720, "Spellcore Engine Window");
		_windowEventChannel->on_window_closed.subscribe(&SandboxApp::CloseApp, this);
		if (!_windowHandle.IsValid())
		{
			LOG_ERROR("Failed to create window");
		}
	}

	void SandboxApp::InitRenderer()
	{
		glm::ivec2 windowSize = _windowSystem.GetWindowSize();
		RenderingSurfaceCreateInfo renderingSurfaceInfo
		{
			_windowHandle.nativeHandle,
			windowSize.x,
			windowSize.y,
			false
		};

		SpellcoreRenderer::Initialize(renderingSurfaceInfo);

		//TODO: Re-fix abuse of abstraction: 
		//Per se, SpellcoreShader is an internal object to the SpellcoreRenderer, so it shouldn't be exposed to the Application layer. 
		//Where as SpellcoreRenderer's UseShader and LoadShader should communicate with the App with just unqiue shader handle. 
		//This way, shader object life time management will be responsiblilty of the SpellcoreRenderer not of Application. 

		//Then there's another concern relating to Resource management. 
		std::string shaderPath = _resourceManager->GetExecutionDir() + "/Resources/DefaultShaders/BasicSpellcoreShader.scsh";
		SpellcoreShader* scShader = SpellcoreRenderer::LoadShader(shaderPath);
		SpellcoreRenderer::UseShader(scShader);
	}

	int SandboxApp::Run()
	{
		LOG_INFO("Starting the SandboxApp");

		if (!_windowHandle.IsValid())
			return -1;

		while (!_closeApp)
		{
			_windowSystem.PollEvents();
			SpellcoreRenderer::BeginFrame();
			_imguiTest.Render();
			SpellcoreRenderer::SubmitMesh(_testMesh); 
			SpellcoreRenderer::EndFrame();
		}

		LOG_INFO("Finished running the sandbox app");

		return 0;
	}

	void SandboxApp::CloseApp()
	{
		_closeApp = true;
	}

	SandboxApp::~SandboxApp()
	{
		_windowEventChannel->on_window_closed.unsubscribe(&SandboxApp::CloseApp, this);
		_windowSystem.DestroyAppWindow(_windowHandle);

		delete _objLoaderTest; 
		delete _testMesh; 
	}
}