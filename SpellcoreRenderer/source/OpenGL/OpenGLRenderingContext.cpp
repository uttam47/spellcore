#include <windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/wglew.h>
#include "OpenGL/OpenGLRenderingContext.h"
#include "Logger.h"
#include <sstream>

namespace AnalyticalApproach::Spellcore
{
    bool OpenGLRenderingContext::Initialize(const RenderingSurfaceCreateInfo &surfaceInfo)
    {
        _windowHandle = static_cast<HWND>(surfaceInfo.nativeWindowHandle);
        _deviceContext = GetDC(_windowHandle);

        // Step 1: Choose and set pixel format
        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR), 1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA, 32,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            24, 8, 0,
            PFD_MAIN_PLANE, 0, 0, 0, 0};

        int pixelFormat = ChoosePixelFormat(_deviceContext, &pfd);
        if (pixelFormat == 0 || !SetPixelFormat(_deviceContext, pixelFormat, &pfd))
        {
            LOG_ERROR("❌ Failed to choose or set pixel format.");
            return false;
        }

        // Step 2: Create dummy context
        HGLRC dummyContext = wglCreateContext(_deviceContext);
        if (!dummyContext || !wglMakeCurrent(_deviceContext, dummyContext))
        {
            LOG_ERROR("❌ Failed to create or activate dummy OpenGL context.");
            return false;
        }

        // Step 3: Init GLEW to access wgl extensions
        if (glewInit() != GLEW_OK)
        {
            LOG_ERROR("❌ glewInit failed on dummy context.");
            return false;
        }

        // Step 4: Load wglCreateContextAttribsARB
        PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
            (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

        if (!wglCreateContextAttribsARB)
        {
            LOG_ERROR("❌ wglCreateContextAttribsARB not available.");
            return false;
        }

        // Step 5: Create real modern OpenGL 4.6 core profile context
        int attribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 6,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            0};

        HGLRC modernContext = wglCreateContextAttribsARB(_deviceContext, nullptr, attribs);
        if (!modernContext)
        {
            LOG_ERROR("❌ Failed to create modern OpenGL context.");
            return false;
        }

        // Step 6: Clean up and switch to modern context
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(dummyContext);
        if (!wglMakeCurrent(_deviceContext, modernContext))
        {
            LOG_ERROR("❌ Failed to activate modern context.");
            return false;
        }

        if (glewInit() != GLEW_OK)
        {
            LOG_ERROR("❌ glewInit failed on real context.");
            return -1;
        }

        PrintGLStatus(); 
        
        _glContext = modernContext;
        return true;
    }

    bool OpenGLRenderingContext::Shutdown()
    {
        bool result = false;
        if (_glContext)
        {
            wglMakeCurrent(nullptr, nullptr);
            wglDeleteContext(_glContext);
            _glContext = nullptr;
            result = true;
        }
        if (_deviceContext && _windowHandle)
        {
            ReleaseDC(_windowHandle, _deviceContext);
            _deviceContext = nullptr;
            _windowHandle = nullptr;
        }
        {
            result = false;
        }

        return result;
    }

    bool OpenGLRenderingContext::BeginFrame()
    {
        glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return true;
    }

    bool OpenGLRenderingContext::EndFrame()
    {
        SwapBuffers(_deviceContext);
        return true;
    }

    void OpenGLRenderingContext::PrintGLStatus()
    {
        LOG_INFO(std::string("----- OpenGL Info -----"));

        const char *version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
        const char *vendor = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
        const char *renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
        const char *glslVer = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));

        LOG_INFO(std::string("🎉 OpenGL Version: ") + (version ? std::string(version) : "Unknown"));
        LOG_INFO(std::string("🏭 GPU Vendor: ") + (vendor ? std::string(vendor) : "Unknown"));
        LOG_INFO(std::string("🖼️  GPU Renderer: ") + (renderer ? std::string(renderer) : "Unknown"));
        LOG_INFO(std::string("📜 GLSL Version: ") + (glslVer ? std::string(glslVer) : "Unknown"));

        GLint numExtensions = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

        if (numExtensions > 0 && glGetStringi != nullptr)
        {
            LOG_INFO(std::string("🔍 Listing OpenGL extensions (core profile):"));
            for (GLint i = 0; i < numExtensions; ++i)
            {
                const char *ext = reinterpret_cast<const char *>(glGetStringi(GL_EXTENSIONS, i));
                if (ext)
                    LOG_INFO(std::string("   ▸ ") + std::string(ext));
            }
        }
        else
        {
            const char *extensions = reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));
            if (extensions)
            {
                LOG_INFO(std::string("🔍 Listing OpenGL extensions (compatibility profile):"));
                std::istringstream extStream(extensions);
                std::string ext;
                while (extStream >> ext)
                {
                    LOG_INFO(std::string("   ▸ ") + ext);
                }
            }
            else
            {
                LOG_INFO(std::string("⚠️  No extensions string available."));
            }
        }

        LOG_INFO(std::string("------------------------"));
    }

}