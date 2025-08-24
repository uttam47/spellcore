#include "ImguiTest.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "WindowSystem/GLFW/GLFWWindowSystem.h"


ImguiTest::ImguiTest()
{
}

ImguiTest::~ImguiTest()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImguiTest::Init(GLFWwindow* glfwWindow)
{
    // Create ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void ImguiTest::Render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (_showDemoWindow)
        ImGui::ShowDemoWindow(&_showDemoWindow);

    {
        ImGui::Begin("Hello from ImguiTest!");
        ImGui::Text("This is a test UI.");
        ImGui::Checkbox("Show Demo Window", &_showDemoWindow);
        ImGui::Checkbox("Show Another Window", &_showAnotherWindow);
        ImGui::ColorEdit3("Clear Color", _clearColor);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
