#include <OpenGL/OpenGLRenderQueue.h>
#include <core/ShaderDataType.h>

#include <algorithm>

namespace AnalyticalApproach::Spellcore
{
	void OpenGLRenderQueue::Submit(RenderPassType pass, const RenderCommand& cmd)
	{
		_renderPassBuckets[pass].push_back(cmd);
	}

	void OpenGLRenderQueue::Execute(RenderPassType pass)
	{
		auto& commands = _renderPassBuckets[pass];

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

	GLenum OpenGLRenderQueue::ToGLIndexType(ShaderDataType t)
	{
		switch (t)
		{
		case ShaderDataType::UShort: return GL_UNSIGNED_SHORT;
		case ShaderDataType::UInt:   return GL_UNSIGNED_INT;
		default:                     return 0;
		}
	}
	GLenum OpenGLRenderQueue::ToGLPrimitiveType(Primitive primitive)
	{
		switch (primitive)
		{
		case Primitive::TRIANGLES: return GL_TRIANGLES;
		case Primitive::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
		case Primitive::TRIANGLE_FAN: return GL_TRIANGLE_FAN;
		case Primitive::LINES: return GL_LINES;
		case Primitive::LINE_STRIP: return GL_LINE_STRIP;
		case Primitive::LINE_LOOP: return GL_LINE_LOOP;
		case Primitive::POINTS: return GL_POINTS;
		case Primitive::PATCHES: return GL_PATCHES;
		}
		return GL_TRIANGLES;
	}

}