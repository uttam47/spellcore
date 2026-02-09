#pragma once

#include <core/components/SCRenderTarget.h>
#include <glm/glm.hpp>
#include <vector>

namespace AnalyticalApproach::Spellcore
{
    class IRenderTarget
    {
    public:
        virtual ~IRenderTarget() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        // Valid means: attachments configured + backend says framebuffer complete (or equivalent)
        virtual bool IsValid() const = 0;

        // Declarative desired size (RT has no size; attachments do). Marks stale until rebuilt.
        virtual void SetSize(glm::ivec2 size) = 0;

        virtual SCRenderTargetHandle GetRenderTargetHandle() const = 0;

        // Store / update the logical description (debug/useful for rebuild decisions)
        virtual void SetDescription(const SCRTDescription& desc) = 0;
        virtual const SCRTDescription& GetDescription() const = 0;

        // Attachment control: slot-based (portable, deterministic)
        virtual void ClearAttachments() = 0;

        virtual void Attach(const SCRTAttachmentBinding& binding) = 0;
        virtual void Detach(const SCAttachmentSlot& slot) = 0;

        // Convenience: build entire RT in one call
        virtual void Build(const SCRTDescription& desc, const std::vector<SCRTAttachmentBinding>& bindings) = 0;
    };

} // namespace AnalyticalApproach::Spellcore