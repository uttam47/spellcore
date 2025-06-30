# Spellcore
Spellcore is a work in progress towards a modular API-Agonistic and extensible renderer for real-time rendering. Scoped for personal research, it is currently being built from the ground up using OpenGL as a base, but with a proper RHI (Rendering Hardware Interaface) abstraction layer to focus on flexibility, shader experimentation, and modern graphics pipeline control.

---

## Key Goals for Spellcore

- 🎯 **Shader-Centric Design**: At the core of Spellcore is a custom `SpellcoreShader` system that will enable shader abstraction.
- 🧱 **Component-Based Render Architecture**: Separation of scene logic and render data through structured pools and registries.
- 🧠 **ECS-Friendly Integration**: Designed to integrate cleanly with external ECS systems like `entt`.
- 🎮 **Real-Time Rendering**: Suitable for interactive applications, visualization, and experimentation.
- 🧪 **Research & Prototyping**: Acts as a rendering testbed for exploring advanced features like clipping planes, render queues, and per-object data pools.
- 🚀 **Minimal External Dependencies**: Uses only essential libraries for OpenGL context, math, and asset loading.

---

## Structure / Organization

- `Spellcore/`
  - `SpellcoreRenderer/`
    - `core/` – Core rendering components (SpellcoreShaders, Materials, Meshes)
    - `RHI/` – Rendering Hardware Interface: Shader Factory (translator, compiler, generators), GPU resources (Buffers, Textures, Shader Programs)
    - `RHI Implementation/` – Backend-specific implementations (OpenGL for now)
  - `SandboxApp/` – Sample app for running and testing Spellcore (may evolve into an editor)
    - `platform/` – Platform/window management (GLFW/GLAD)
  - `RunesUI/` – A customized UI library, planned to integrate on top of Spellcore
  - `ThirdPartyLibs/` – GLEW, GLFW, glm, ImGui, spdlog (managed via submodules)
  - `README.md` – This file

---
## Build Process

### Prerequisites

- A C++17 compatible compiler (MSVC, Clang, or GCC)
- [CMake](https://cmake.org/) ≥ 3.16
- Git

### Clone and Build

```bash
# Clone the repository with submodules
git clone --recurse-submodules https://github.com/yourusername/spellcore.git
cd spellcore

# Create build directory
mkdir build && cd build

# Generate project files
cmake ..

# Build
cmake --build .
