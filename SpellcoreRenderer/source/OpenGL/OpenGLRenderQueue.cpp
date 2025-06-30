#include <OpenGL/OpenGLRenderQueue.h>
#include <GL/glew.h>

#include <algorithm>

namespace AnalyticalApproach::Spellcore
{
    void OpenGLRenderQueue::Submit(RenderPassType pass, const RenderCommand &cmd)
    {
        _renderPassBuckets[pass].push_back(cmd);
    }

    void OpenGLRenderQueue::Execute(RenderPassType pass)
    {
        auto &commands = _renderPassBuckets[pass];

        // Sort to reduce state changes (shaderID major, vaoID minor)
        std::sort(commands.begin(), commands.end(),
                  [](const RenderCommand &a, const RenderCommand &b)
                  {
                      return a.sortKey < b.sortKey;
                  });

        uint32_t lastShader = 0xFFFFFFFF;
        uint32_t lastVAO = 0xFFFFFFFF;

        for (const auto &cmd : commands)
        {
            if (cmd.shaderID != lastShader)
            {
                glUseProgram(cmd.shaderID);
                lastShader = cmd.shaderID;
            }

            if (cmd.vaoID != lastVAO)
            {
                glBindVertexArray(cmd.vaoID);
                lastVAO = cmd.vaoID;
            }

            glDrawElements(GL_TRIANGLES, cmd.indexCount, GL_UNSIGNED_INT, nullptr);
        }

        commands.clear(); // Clear after execution
    }

    void OpenGLRenderQueue::Clear()
    {
        for (auto &[_, commands] : _renderPassBuckets)
            commands.clear();
    }
}