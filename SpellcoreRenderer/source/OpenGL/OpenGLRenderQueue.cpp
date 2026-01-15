#include <algorithm>
#include <OpenGL/OpenGLRenderQueue.h>


namespace AnalyticalApproach::Spellcore
{
	void OpenGLRenderQueue::Submit(const SCRenderPassHandle& scrpHandle, const RenderCommand& cmd)
	{
		_renderPassBuckets[scrpHandle].push_back(cmd);
	}

	void OpenGLRenderQueue::Execute(const SCRenderPassHandle& scrpHandle)
	{
		auto& commands = _renderPassBuckets[scrpHandle];

		std::sort(commands.begin(), commands.end(),
			[](const RenderCommand& a, const RenderCommand& b)
			{
				return a.sortKey < b.sortKey;
			});

		GLuint lastProgram = 0;
		GLuint lastVAO = 0;

		for (const auto& cmd : commands)
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

		commands.clear();
	}
	void OpenGLRenderQueue::Clear()
	{
		for (auto& [_, commands] : _renderPassBuckets)
			commands.clear();
	}

	GLenum OpenGLRenderQueue::ToGLIndexType(SCDataType t)
	{
		switch (t)
		{
		case SCDataType::UShort: return GL_UNSIGNED_SHORT;
		case SCDataType::UInt:   return GL_UNSIGNED_INT;
		default:                     return 0;
		}
	}
	GLenum OpenGLRenderQueue::ToGLPrimitiveType(SCPrimitive primitive)
	{
		switch (primitive)
		{
		case SCPrimitive::TRIANGLES: return GL_TRIANGLES;
		case SCPrimitive::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
		case SCPrimitive::TRIANGLE_FAN: return GL_TRIANGLE_FAN;
		case SCPrimitive::LINES: return GL_LINES;
		case SCPrimitive::LINE_STRIP: return GL_LINE_STRIP;
		case SCPrimitive::LINE_LOOP: return GL_LINE_LOOP;
		case SCPrimitive::POINTS: return GL_POINTS;
		case SCPrimitive::PATCHES: return GL_PATCHES;
		}
		return GL_TRIANGLES;
	}

}