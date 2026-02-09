#pragma once

#include <RHI/IRenderTarget.h>

#include <unordered_map>
#include <vector>

namespace AnalyticalApproach::Spellcore
{
    class OpenGLRenderTarget final : public IRenderTarget
    {
    public:
        OpenGLRenderTarget();
        ~OpenGLRenderTarget() override;

        void Bind() override;
        void Unbind() override;

        bool IsValid() const override;
        void SetSize(glm::ivec2 size) override;

        SCRenderTargetHandle GetRenderTargetHandle() const override;

        void SetDescription(const SCRTDescription& desc) override;
        const SCRTDescription& GetDescription() const override;

        void ClearAttachments() override;
        void Attach(const SCRTAttachmentBinding& binding) override;
        void Detach(const SCAttachmentSlot& slot) override;

        void Build(const SCRTDescription& desc, const std::vector<SCRTAttachmentBinding>& bindings) override;

    private:
        struct SlotKey
        {
            SCRTAttachmentType type = SCRTAttachmentType::Color;
            uint32_t index = 0;

            bool operator==(const SlotKey& o) const noexcept
            {
                return type == o.type && index == o.index;
            }
        };

        struct SlotKeyHash
        {
            std::size_t operator()(const SlotKey& k) const noexcept
            {
                return (static_cast<std::size_t>(k.index) << 8) ^ static_cast<std::size_t>(k.type);
            }
        };

        bool _isValid = false;
        bool _needsRebuild = false;

        // Logical description (backend-agnostic)
        SCRTDescription _desc{};

        // Desired render-target size (declarative)
        glm::ivec2 _size{ 0, 0 };

        // Actual GL framebuffer name (store as GLuint-sized)
        unsigned int _fboGL = 0;

        // Current bindings, keyed by attachment slot (portable and replacement-friendly)
        std::unordered_map<SlotKey, SCRTAttachmentBinding, SlotKeyHash> _slotBindings;

    private:
        // Helpers
        static bool IsDepthStencilFormat(SCRTFormat fmt);
        const SCAttachmentDesc* FindAttachmentDesc(const SCAttachmentSlot& slot) const;

        void DetachGLAttachmentForSlot(const SCAttachmentSlot& slot);
        void UpdateDrawBuffers();
        void ValidateFramebufferComplete(bool assertOnFail);

        // Attach helpers
        void AttachRenderbuffer(unsigned int rb, unsigned int glAttachment);
        void AttachTexture(const SCAttachmentDesc& attDesc,
            unsigned int tex,
            unsigned int glAttachment,
            const SCImageViewDesc& view);

        // Size validation (OpenGL-side query)
        bool QueryTextureLevelSize(unsigned int tex, uint32_t mipLevel, int& outW, int& outH) const;
        bool QueryRenderbufferSize(unsigned int rb, int& outW, int& outH) const;
        void ValidateAttachmentSizeAgainstTarget(int attW, int attH);

        // Handle classification
        static bool IsGLTexture(unsigned int name);
        static bool IsGLRenderbuffer(unsigned int name);
        
    };

} // namespace AnalyticalApproach::Spellcore