#include <OpenGL/OpenGLTextureImage.h>
#include <OpenGL/OpenGLTypeUtility.h>
#include <cassert>

namespace AnalyticalApproach::Spellcore
{
    
    OpenGLTextureImage::OpenGLTextureImage(const SCImageDesc& textureImageDesc)
        : _textureImageHandle(0)
    {
        assert(textureImageDesc.width  > 0);
        assert(textureImageDesc.height > 0);

        // If your struct uses a different name than renderFormat, change this line:
        const GLenum internalFormat = ToGLInternalFormat(textureImageDesc.format);
        assert(internalFormat != 0);

        const int samples = ToGLSampleCount(textureImageDesc.sampleCount);

        if (samples > 1)
        {
            // Multisample render-target texture
            glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &_textureImageHandle);

            // fixedSampleLocations = GL_TRUE is the typical default
            glTextureStorage2DMultisample(
                _textureImageHandle,
                samples,
                internalFormat,
                static_cast<GLsizei>(textureImageDesc.width),
                static_cast<GLsizei>(textureImageDesc.height),
                GL_TRUE
            );

            // Note: multisample textures ignore filtering/wrap params.
        }
        else
        {
            glCreateTextures(GL_TEXTURE_2D, 1, &_textureImageHandle);

            int mipLevels = static_cast<uint8_t>(textureImageDesc.mipLevels); 
            mipLevels = mipLevels > 0 ? mipLevels : 1;

            glTextureStorage2D(
                _textureImageHandle,
                mipLevels,
                internalFormat,
                static_cast<GLsizei>(textureImageDesc.width),
                static_cast<GLsizei>(textureImageDesc.height)
            );

            // Defaults that are usually correct for render targets (GBuffer, depth, etc.)
            glTextureParameteri(_textureImageHandle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureParameteri(_textureImageHandle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTextureParameteri(_textureImageHandle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(_textureImageHandle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // If you later use it as a shadow map, you'd set compare mode here.
            // glTextureParameteri(_textureImageHandle, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            // glTextureParameteri(_textureImageHandle, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        }
    }

    OpenGLTextureImage::~OpenGLTextureImage()
    {
        if (_textureImageHandle != 0)
        {
            glDeleteTextures(1, &_textureImageHandle);
            _textureImageHandle = 0;
        }
    }

    SCImageHandle OpenGLTextureImage::GetTextureHandle()
    {
        return _textureImageHandle;
    }
}