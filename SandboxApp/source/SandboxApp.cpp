#include <filesystem>
#include <core/components/RenderingSurfaceCreateInfo.h>
#include <core/components/SCGeometryData.h>

#include <SandboxApp.h>

namespace AnalyticalApproach::SpellcoreEditor
{
	using namespace WindowSystem;
	using namespace EventSystem;
	using namespace Spellcore; 

	SandboxApp::SandboxApp(std::vector<std::string> appParameters)
	{
		_resourceRegistry = new ResourceManager(std::filesystem::path(appParameters[0]).parent_path().string());

		InitAppWindow(); 
		InitRenderer();		
		TestingHelper(); 
	}

	void SandboxApp::TestingHelper()
	{
		//Imgui Test object
		_imguiTest.Init(_windowHandle.glfwWindow);

		_objLoaderTest = new ObjLoaderTest(); 

		_geometryCPUCache = _objLoaderTest->GetCubeMesh();
		_testMeshHandle = _renderResourceRegistry->CreateGeometryResource(*_geometryCPUCache);

		//SpellcoreRenderer::SubmitRenderCommand(_testMeshHandle, "Base_Layer");


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
		_renderResourceRegistry = RenderResourceRegistry::GetInstance();

		//TODO: Re-fix abuse of abstraction: 
		//Per se, SpellcoreShader is an internal object to the SpellcoreRenderer, so it shouldn't be exposed to the Application layer. 
		//Where as SpellcoreRenderer's UseShader and LoadShader should communicate with the App with just unqiue shader handle. 
		//This way, shader object life time management will be responsiblilty of the SpellcoreRenderer not of Application. 

		//Then there's another concern relating to Resource management. 
		std::string shaderPath = _resourceRegistry->GetExecutionDir() + "/Resources/DefaultShaders/BasicSpellcoreShader.scsh";
		_testShader = SpellcoreRenderer::LoadShader(shaderPath);
		SpellcoreRenderer::UseShader(_testShader);
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
			//SpellcoreRenderer::SubmitRenderCommand(_testMeshHandle,"BASE");
			SpellcoreRenderer::RenderFrame(); 
			_imguiTest.Render();
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

		_renderResourceRegistry->DestroyGeometryResource(_testMeshHandle); 

		delete _geometryCPUCache; 
		delete _objLoaderTest; 
	}
}