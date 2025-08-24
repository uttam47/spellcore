#pragma once
#include <string>

#include <GLFW/glfw3.h>

class ImguiTest
{
public:
    ImguiTest();
    ~ImguiTest();

    void Init(GLFWwindow* glfwWindow);   // Call once after SpellcoreRenderer is initialized
    void Render(); // Call every frame between BeginFrame() / EndFrame()

private:
    bool _showDemoWindow = true;
    bool _showAnotherWindow = false;
    float _clearColor[4] = { 0.45f, 0.55f, 0.60f, 1.00f };
};
