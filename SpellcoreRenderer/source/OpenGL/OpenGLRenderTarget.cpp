#include <OpenGL/OpenGLRenderTarget.h>

#include <GL/glew.h>

#include <algorithm>
#include <cassert>
#include <vector>

namespace AnalyticalApproach::Spellcore
{
    static GLenum ToGLColorAttachment(uint32_t index)
    {
        return static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + index);
    }

    bool OpenGLRenderTarget::IsDepthStencilFormat(SCRTFormat fmt)
    {
        return fmt == SCRTFormat::D24S8;
    }

    bool OpenGLRenderTarget::IsGLTexture(unsigned int name)
    {
        return name != 0 && glIsTexture(static_cast<GLuint>(name)) == GL_TRUE;
    }

    bool OpenGLRenderTarget::IsGLRenderbuffer(unsigned int name)
    {
        return name != 0 && glIsRenderbuffer(static_cast<GLuint>(name)) == GL_TRUE;
    }

    const SCAttachmentDesc* OpenGLRenderTarget::FindAttachmentDesc(const SCAttachmentSlot& slot) const
    {
        // Assumes SCRTDescription has: std::vector<SCAttachmentDesc> attachments
        for (const auto& a : _desc.attachments)
        {
            if (a.attachmentType != slot.type)
                continue;

            if (slot.type == SCRTAttachmentType::Color)
            {
                if (a.attachmentIndex == slot.index)
                    return &a;
            }
            else
            {
                // Depth/Stencil singleton
                return &a;
            }
        }
        return nullptr;
    }

    OpenGLRenderTarget::OpenGLRenderTarget()
    {
        glCreateFramebuffers(1, reinterpret_cast<GLuint*>(&_fboGL));
        assert(_fboGL != 0 && "OpenGLRenderTarget: Failed to create framebuffer");

        _isValid = false;
        _needsRebuild = false;
        _size = { 0, 0 };
    }

    OpenGLRenderTarget::~OpenGLRenderTarget()
    {
        _slotBindings.clear();
        _isValid = false;

        if (_fboGL != 0)
        {
            glDeleteFramebuffers(1, reinterpret_cast<GLuint*>(&_fboGL));
            _fboGL = 0;
        }
    }

    void OpenGLRenderTarget::Bind()
    {
        assert(_fboGL != 0);
        assert(_isValid && !_needsRebuild && "OpenGLRenderTarget: binding invalid/stale render target (size changed or incomplete)");
        glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(_fboGL));
    }

    void OpenGLRenderTarget::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    bool OpenGLRenderTarget::IsValid() const
    {
        return _isValid && !_needsRebuild;
    }

    void OpenGLRenderTarget::SetSize(glm::ivec2 size)
    {
        size.x = std::max(size.x, 0);
        size.y = std::max(size.y, 0);

        if (_size == size)
            return;

        _size = size;

        // Declarative: caller must recreate / resize images and then re-attach.
        if (!_slotBindings.empty())
        {
            _needsRebuild = true;
            _isValid = false;
        }
    }

    SCRenderTargetHandle OpenGLRenderTarget::GetRenderTargetHandle() const
    {
        // IMPORTANT: SCRenderTargetHandle must be >= 32-bit if you return GL object names.
        return static_cast<SCRenderTargetHandle>(_fboGL);
    }

    void OpenGLRenderTarget::SetDescription(const SCRTDescription& desc)
    {
        _desc = desc;

        if (desc.width > 0 && desc.height > 0)
            SetSize(glm::ivec2(static_cast<int>(desc.width), static_cast<int>(desc.height)));

        if (!_slotBindings.empty())
        {
            _needsRebuild = true;
            _isValid = false;
        }
    }

    const SCRTDescription& OpenGLRenderTarget::GetDescription() const
    {
        return _desc;
    }

    void OpenGLRenderTarget::ClearAttachments()
    {
        if (_fboGL == 0)
            return;

        for (const auto& kv : _slotBindings)
        {
            const SlotKey& k = kv.first;
            SCAttachmentSlot slot{ k.type, k.index };
            DetachGLAttachmentForSlot(slot);
        }

        _slotBindings.clear();

        UpdateDrawBuffers();
        _isValid = false;
        _needsRebuild = false;
    }

    void OpenGLRenderTarget::Build(const SCRTDescription& desc, const std::vector<SCRTAttachmentBinding>& bindings)
    {
        SetDescription(desc);
        ClearAttachments();

        for (const auto& b : bindings)
            Attach(b);

        UpdateDrawBuffers();
        ValidateFramebufferComplete(true);
        _needsRebuild = false;
    }

    void OpenGLRenderTarget::Detach(const SCAttachmentSlot& slotIn)
    {
        if (_fboGL == 0)
            return;

        SCAttachmentSlot slot = slotIn;
        if (slot.type != SCRTAttachmentType::Color)
            slot.index = 0;

        const SlotKey key{ slot.type, slot.index };
        auto it = _slotBindings.find(key);
        if (it == _slotBindings.end())
            return;

        DetachGLAttachmentForSlot(slot);
        _slotBindings.erase(it);

        UpdateDrawBuffers();
        ValidateFramebufferComplete(false);
    }

    void OpenGLRenderTarget::Attach(const SCRTAttachmentBinding& bindingIn)
    {
        assert(_fboGL != 0);
        assert(bindingIn.imageHandle != 0);

        SCRTAttachmentBinding binding = bindingIn;
        if (binding.slot.type != SCRTAttachmentType::Color)
            binding.slot.index = 0;

        const SCAttachmentDesc* attDescPtr = FindAttachmentDesc(binding.slot);
        assert(attDescPtr && "OpenGLRenderTarget::Attach: binding.slot not found in SCRTDescription");
        if (!attDescPtr)
        {
            _isValid = false;
            _needsRebuild = true;
            return;
        }
        const SCAttachmentDesc& attDesc = *attDescPtr;

        // Replace existing binding in that slot
        const SlotKey key{ binding.slot.type, binding.slot.index };
        if (_slotBindings.find(key) != _slotBindings.end())
            Detach(binding.slot);

        // Decide GL attachment enum
        GLenum glAttachment = GL_NONE;

        if (binding.slot.type == SCRTAttachmentType::Color)
        {
            glAttachment = ToGLColorAttachment(binding.slot.index);
        }
        else
        {
            if (IsDepthStencilFormat(attDesc.image.format))
                glAttachment = GL_DEPTH_STENCIL_ATTACHMENT;
            else if (binding.slot.type == SCRTAttachmentType::Depth)
                glAttachment = GL_DEPTH_ATTACHMENT;
            else
                glAttachment = GL_STENCIL_ATTACHMENT;

            if (glAttachment == GL_DEPTH_STENCIL_ATTACHMENT)
            {
                // Supersedes both depth and stencil bookkeeping
                _slotBindings.erase(SlotKey{ SCRTAttachmentType::Depth, 0 });
                _slotBindings.erase(SlotKey{ SCRTAttachmentType::Stencil, 0 });

                // Detach conflicting points before attaching new one
                glNamedFramebufferTexture(static_cast<GLuint>(_fboGL), GL_DEPTH_ATTACHMENT, 0, 0);
                glNamedFramebufferTexture(static_cast<GLuint>(_fboGL), GL_STENCIL_ATTACHMENT, 0, 0);
                glNamedFramebufferTexture(static_cast<GLuint>(_fboGL), GL_DEPTH_STENCIL_ATTACHMENT, 0, 0);

                glNamedFramebufferRenderbuffer(static_cast<GLuint>(_fboGL), GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
                glNamedFramebufferRenderbuffer(static_cast<GLuint>(_fboGL), GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
                glNamedFramebufferRenderbuffer(static_cast<GLuint>(_fboGL), GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
            }
        }

        const GLuint name = static_cast<GLuint>(binding.imageHandle);

        const bool isRB = IsGLRenderbuffer(binding.imageHandle);
        const bool isTex = IsGLTexture(binding.imageHandle);
        assert((isRB || isTex) && "OpenGLRenderTarget::Attach: imageHandle is neither a texture nor a renderbuffer");

        if (isRB)
        {
            assert(binding.view.mipLevel == 0 && !binding.view.layered && binding.view.layer == 0 && binding.view.slice == 0);
            AttachRenderbuffer(name, static_cast<unsigned int>(glAttachment));

            int w = 0, h = 0;
            if (QueryRenderbufferSize(name, w, h))
                ValidateAttachmentSizeAgainstTarget(w, h);
        }
        else
        {
            AttachTexture(attDesc, name, static_cast<unsigned int>(glAttachment), binding.view);

            int w = 0, h = 0;
            if (QueryTextureLevelSize(name, binding.view.mipLevel, w, h))
                ValidateAttachmentSizeAgainstTarget(w, h);
        }

        _slotBindings[key] = binding;

        UpdateDrawBuffers();
        ValidateFramebufferComplete(false);

        if (_isValid)
            _needsRebuild = false;
    }

    void OpenGLRenderTarget::DetachGLAttachmentForSlot(const SCAttachmentSlot& slot)
    {
        const SCAttachmentDesc* attDescPtr = FindAttachmentDesc(slot);
        const bool wantsDepthStencil = (attDescPtr != nullptr) ? IsDepthStencilFormat(attDescPtr->image.format) : false;

        auto DetachPoint = [&](GLenum a)
            {
                glNamedFramebufferTexture(static_cast<GLuint>(_fboGL), a, 0, 0);
                glNamedFramebufferRenderbuffer(static_cast<GLuint>(_fboGL), a, GL_RENDERBUFFER, 0);
            };

        if (slot.type == SCRTAttachmentType::Color)
        {
            DetachPoint(ToGLColorAttachment(slot.index));
            return;
        }

        if (wantsDepthStencil)
        {
            DetachPoint(GL_DEPTH_STENCIL_ATTACHMENT);
            DetachPoint(GL_DEPTH_ATTACHMENT);
            DetachPoint(GL_STENCIL_ATTACHMENT);
            return;
        }

        if (slot.type == SCRTAttachmentType::Depth)
            DetachPoint(GL_DEPTH_ATTACHMENT);
        else
            DetachPoint(GL_STENCIL_ATTACHMENT);

        DetachPoint(GL_DEPTH_STENCIL_ATTACHMENT);
    }

    void OpenGLRenderTarget::AttachRenderbuffer(unsigned int rb, unsigned int glAttachment)
    {
        glNamedFramebufferRenderbuffer(static_cast<GLuint>(_fboGL),
            static_cast<GLenum>(glAttachment),
            GL_RENDERBUFFER,
            static_cast<GLuint>(rb));
    }

    void OpenGLRenderTarget::AttachTexture(const SCAttachmentDesc& attDesc,
        unsigned int tex,
        unsigned int glAttachment,
        const SCImageViewDesc& view)
    {
        const GLuint fbo = static_cast<GLuint>(_fboGL);
        const GLuint t = static_cast<GLuint>(tex);
        const GLenum a = static_cast<GLenum>(glAttachment);
        const GLint level = static_cast<GLint>(view.mipLevel);

        if (view.layered)
        {
            glNamedFramebufferTexture(fbo, a, t, level);
            return;
        }

        const SCImageType type = attDesc.image.type;

        if (type == SCImageType::Tex2D || type == SCImageType::Tex1D)
        {
            glNamedFramebufferTexture(fbo, a, t, level);
            return;
        }

        if (type == SCImageType::TexCube)
        {
            // face: 0..5 in view.layer
            const uint32_t face = view.layer;
            assert(face < 6);

            GLint prev = 0;
            glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prev);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);

            glFramebufferTexture2D(GL_FRAMEBUFFER,
                a,
                static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face),
                t,
                level);

            glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(prev));
            return;
        }

        // Array / 3D / cube array
        GLuint layer = 0;
        if (type == SCImageType::Tex3D)
            layer = static_cast<GLuint>(view.slice);
        else
            layer = static_cast<GLuint>(view.layer);

        glNamedFramebufferTextureLayer(fbo, a, t, level, static_cast<GLint>(layer));
    }

    void OpenGLRenderTarget::UpdateDrawBuffers()
    {
        const GLuint fbo = static_cast<GLuint>(_fboGL);
        if (fbo == 0)
            return;

        int32_t maxColor = -1;
        for (const auto& kv : _slotBindings)
        {
            const SlotKey& k = kv.first;
            if (k.type == SCRTAttachmentType::Color)
                maxColor = std::max(maxColor, static_cast<int32_t>(k.index));
        }

        if (maxColor < 0)
        {
            glNamedFramebufferDrawBuffer(fbo, GL_NONE);
            glNamedFramebufferReadBuffer(fbo, GL_NONE);
            return;
        }

        std::vector<GLenum> bufs(static_cast<size_t>(maxColor) + 1, GL_NONE);
        for (const auto& kv : _slotBindings)
        {
            const SlotKey& k = kv.first;
            if (k.type != SCRTAttachmentType::Color)
                continue;

            bufs[k.index] = ToGLColorAttachment(k.index);
        }

        glNamedFramebufferDrawBuffers(fbo, static_cast<GLsizei>(bufs.size()), bufs.data());

        const auto it = std::find_if(bufs.begin(), bufs.end(), [](GLenum e) { return e != GL_NONE; });
        glNamedFramebufferReadBuffer(fbo, (it != bufs.end()) ? *it : GL_NONE);
    }

    void OpenGLRenderTarget::ValidateFramebufferComplete(bool assertOnFail)
    {
        const GLuint fbo = static_cast<GLuint>(_fboGL);

        if (fbo == 0 || _slotBindings.empty())
        {
            _isValid = false;
            return;
        }

        const GLenum status = glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER);
        _isValid = (status == GL_FRAMEBUFFER_COMPLETE);

        if (assertOnFail)
            assert(_isValid && "OpenGLRenderTarget: Framebuffer incomplete");
    }

    bool OpenGLRenderTarget::QueryTextureLevelSize(unsigned int tex, uint32_t mipLevel, int& outW, int& outH) const
    {
        if (!IsGLTexture(tex))
            return false;

        GLint w = 0, h = 0;
        glGetTextureLevelParameteriv(static_cast<GLuint>(tex), static_cast<GLint>(mipLevel), GL_TEXTURE_WIDTH, &w);
        glGetTextureLevelParameteriv(static_cast<GLuint>(tex), static_cast<GLint>(mipLevel), GL_TEXTURE_HEIGHT, &h);

        outW = w;
        outH = h;
        return (w > 0 && h > 0);
    }

    bool OpenGLRenderTarget::QueryRenderbufferSize(unsigned int rb, int& outW, int& outH) const
    {
        if (!IsGLRenderbuffer(rb))
            return false;

        GLint w = 0, h = 0;
        glGetNamedRenderbufferParameteriv(static_cast<GLuint>(rb), GL_RENDERBUFFER_WIDTH, &w);
        glGetNamedRenderbufferParameteriv(static_cast<GLuint>(rb), GL_RENDERBUFFER_HEIGHT, &h);

        outW = w;
        outH = h;
        return (w > 0 && h > 0);
    }

    void OpenGLRenderTarget::ValidateAttachmentSizeAgainstTarget(int attW, int attH)
    {
        if (attW <= 0 || attH <= 0)
            return;

        if (_size.x <= 0 || _size.y <= 0)
        {
            _size = { attW, attH };
            return;
        }

        if (_size.x != attW || _size.y != attH)
        {
            _isValid = false;
            _needsRebuild = true;
            assert(false && "OpenGLRenderTarget: attachment size != render target size; recreate attachments and reattach");
        }
    }

} // namespace AnalyticalApproach::Spellcore