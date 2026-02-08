#include <OpenGL/OpenGLRenderTarget.h>
#include <OpenGL/OpenGLTypeUtility.h>
#include <cassert>
#include <algorithm>

namespace AnalyticalApproach::Spellcore
{

    void OpenGLRenderTarget::ValidateFramebufferComplete()
    {
        const GLenum status = glCheckNamedFramebufferStatus(_fboHandle, GL_FRAMEBUFFER);
        _isValid = status == GL_FRAMEBUFFER_COMPLETE; 
        assert(_isValid && "OpenGLRenderTarget: Framebuffer is incomplete!");
    }

    void OpenGLRenderTarget::UpdateDrawBuffers()
    {
        // Find maximum color attachment index currently bound
        int32_t maxColorIndex = -1;
        for (const auto& [imgHandle, att] : _bindings)
        {
            if (att.attachment == SCRTAttachmentType::Color)
                maxColorIndex = std::max(maxColorIndex, static_cast<int32_t>(att.colorIndex));
        }

        if (maxColorIndex < 0)
        {
            // Depth-only / stencil-only target
            glNamedFramebufferDrawBuffer(_fboHandle, GL_NONE);
            glNamedFramebufferReadBuffer(_fboHandle, GL_NONE);
            return;
        }

        // IMPORTANT:
        // We build a dense array [0..maxColorIndex] so that output location i maps to attachment i.
        std::vector<GLenum> bufs(static_cast<size_t>(maxColorIndex) + 1, GL_NONE);

        for (const auto& [imgHandle, att] : _bindings)
        {
            if (att.attachment != SCRTAttachmentType::Color)
                continue;

            const uint32_t i = att.colorIndex;
            if (i > static_cast<uint32_t>(maxColorIndex))
                continue;

            bufs[i] = static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + i);
        }

        glNamedFramebufferDrawBuffers(
            _fboHandle,
            static_cast<GLsizei>(bufs.size()),
            bufs.data()
        );

        // Read buffer: pick the first valid color attachment
        const auto it = std::find_if(bufs.begin(), bufs.end(),
            [](GLenum e) { return e != GL_NONE; });

        glNamedFramebufferReadBuffer(_fboHandle, (it != bufs.end()) ? *it : GL_NONE);
    }

    // ---------------------------
    // OpenGLRenderTarget
    // ---------------------------

    OpenGLRenderTarget::OpenGLRenderTarget()
    {
        glCreateFramebuffers(1, &_fboHandle);
        assert(_fboHandle != 0 && "Failed to create framebuffer!");
    }

    void OpenGLRenderTarget::RemoveAttachment(const SCImageHandle& imageHandle)
    {
        if (_fboHandle == 0)
            return;

        auto it = _bindings.find(imageHandle);
        if (it == _bindings.end())
            return;

        const Attachment att = it->second;

        auto Detach = [&](GLenum glAttachment)
            {
                // Detach both ways safely (only one will matter based on what was attached)
                glNamedFramebufferTexture(_fboHandle, glAttachment, 0, 0);
                glNamedFramebufferRenderbuffer(_fboHandle, glAttachment, GL_RENDERBUFFER, 0);
            };

        switch (att.attachment)
        {
        case SCRTAttachmentType::Color:
        {
            const GLenum glAttachment = static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + att.colorIndex);
            Detach(glAttachment);
            break;
        }
        case SCRTAttachmentType::Depth:
        {
            // Could have been attached as DEPTH or DEPTH_STENCIL (D24S8 etc.)
            Detach(GL_DEPTH_ATTACHMENT);
            Detach(GL_DEPTH_STENCIL_ATTACHMENT);
            break;
        }
        case SCRTAttachmentType::Stencil:
        {
            // Could have been attached as STENCIL or DEPTH_STENCIL
            Detach(GL_STENCIL_ATTACHMENT);
            Detach(GL_DEPTH_STENCIL_ATTACHMENT);
            break;
        }
        default:
            // If you add more attachment types later
            break;
        }

        _bindings.erase(it);

        UpdateDrawBuffers();
        ValidateFramebufferComplete();
    }

    bool OpenGLRenderTarget::IsValid()
    {
        return _isValid; 
    }

    OpenGLRenderTarget::~OpenGLRenderTarget()
    {
        
        for (auto attachment : _bindings)
        {

        }

        if (_fboHandle != 0)
        {
            glDeleteFramebuffers(1, &_fboHandle);
            _fboHandle = 0;
        }
    }

    void OpenGLRenderTarget::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _fboHandle);
    }

    void OpenGLRenderTarget::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLRenderTarget::AddRenderAttachment(const SCImageHandle& imageHandle, const SCRenderImageDesc& renderImageDesc)
    {
        assert(_fboHandle != 0);
        assert(renderImageDesc.width > 0 && renderImageDesc.height > 0);

       
        const GLenum attachment = static_cast<GLenum>(ToGLAttachmentPoint(renderImageDesc));

        // Attach RBO to this FBO
        glNamedFramebufferRenderbuffer(
            _fboHandle,
            attachment,
            GL_RENDERBUFFER,
            static_cast<GLuint>(imageHandle)
        );

        Attachment at{};

        at.handle = imageHandle;
        at.isRenderbuffer = false;
        at.attachment = renderImageDesc.attachmentType;
        at.colorIndex = renderImageDesc.attachmentIndex;
        _bindings[imageHandle] = at;


        UpdateDrawBuffers();
        ValidateFramebufferComplete();
    }

    void OpenGLRenderTarget::AddRenderAttachment(const SCImageHandle& imageHandle, const SCTextureImageDesc& textureImageDesc)
    {
        assert(_fboHandle != 0);
        assert(textureImageDesc.width > 0 && textureImageDesc.height > 0);


        const GLenum attachment = static_cast<GLenum>(ToGLAttachmentPoint(textureImageDesc));

        // Attach texture to this FBO (works for 2D and 2D MSAA)
        glNamedFramebufferTexture(
            _fboHandle,
            attachment,
            static_cast<GLuint>(imageHandle),
            0 // mip level (ignored for MSAA)
        );

        
        Attachment at{}; 

        at.handle = imageHandle; 
        at.isRenderbuffer = false;
        at.attachment = textureImageDesc.attachmentType;
        at.colorIndex = textureImageDesc.attachmentIndex; 
        _bindings[imageHandle] = at; 


        UpdateDrawBuffers();
        ValidateFramebufferComplete();
    }

    uint32_t OpenGLRenderTarget::GetRenderTargetHandle()
    {
        return _fboHandle;
    }

} // namespace AnalyticalApproach::Spellcore