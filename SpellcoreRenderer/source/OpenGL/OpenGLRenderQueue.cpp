#include <algorithm>
#include <OpenGL/OpenGLRenderQueue.h>
#include <OpenGL/OpenGLTypeUtility.h> 

namespace AnalyticalApproach::Spellcore
{
	void OpenGLRenderQueue::Submit( const RenderCommand& cmd)
	{
		_commands.push_back(cmd);
	}

	void OpenGLRenderQueue::Execute()
	{
		std::sort(_commands.begin(), _commands.end(),
			[](const RenderCommand& a, const RenderCommand& b)
			{
				return a.sortKey < b.sortKey;
			});

		GLuint lastProgram = 0;
		GLuint lastVAO = 0;

		for (const auto& cmd : _commands)
		{
			if (cmd.elementCount == 0)
				continue;

			if (cmd.pipelineID != lastProgram)
			{
				glUseProgram(cmd.pipelineID);
				lastProgram = cmd.pipelineID;
			}

			if (cmd.geometryId != lastVAO)
			{
				glBindVertexArray(cmd.geometryId);
				lastVAO = cmd.geometryId;
			}

			const GLenum mode = ToGLPrimitiveType(cmd.primitive);
			const GLenum glIndexType = ToGLIndexType(cmd.indexType);

			if (glIndexType != 0)
			{
				glDrawElements(mode, cmd.elementCount, glIndexType, nullptr);
			}
			else
			{
				glDrawArrays(mode, /*firstVertex*/ 0, cmd.elementCount);
			}
		}

		_commands.clear();
	}

	void OpenGLRenderQueue::Clear()
	{
		_commands.clear();
	}

	void OpenGLRenderQueue::Sort()
	{
		//sort. 
	}
}