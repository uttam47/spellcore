#include <GL/glew.h>
#include <OpenGL/OpenGLTypeUtility.h>
#include <OpenGL/OpenGLRenderBufferObject.h>

namespace AnalyticalApproach::Spellcore
{

    OpenGLRenderBufferObject::OpenGLRenderBufferObject(const SCRenderImageDesc& renderImgDescription)
        : _rboHandle(0)
    {
        assert(renderImgDescription.width > 0);
        assert(renderImgDescription.height > 0);

        const GLenum internalFormat = ToGLInternalFormat(renderImgDescription.renderFormat);

        assert(internalFormat != 0);

        glCreateRenderbuffers(1, &_rboHandle);

        const int samples = ToGLSampleCount(renderImgDescription.sampleCount);

        if (samples > 1)
        {
            glNamedRenderbufferStorageMultisample(
                _rboHandle,
                samples,
                internalFormat,
                static_cast<GLsizei>(renderImgDescription.width),
                static_cast<GLsizei>(renderImgDescription.height)
            );
        }
        else
        {
            glNamedRenderbufferStorage(
                _rboHandle,
                internalFormat,
                static_cast<GLsizei>(renderImgDescription.width),
                static_cast<GLsizei>(renderImgDescription.height)
            );
        }
    }

    OpenGLRenderBufferObject::~OpenGLRenderBufferObject()
    {
        if (_rboHandle != 0)
        {
            glDeleteRenderbuffers(1, &_rboHandle);
            _rboHandle = 0;
        }
    }

    SCImageHandle OpenGLRenderBufferObject::GetRBOHandle()
    {
        return _rboHandle;
    }
}