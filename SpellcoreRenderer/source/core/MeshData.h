#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <utility>
#include <algorithm>
#include <cstring> // std::memcpy
#include "ShaderDataType.h"
#include "GPUBufferLayout.h"

namespace AnalyticalApproach::Spellcore
{
    // Packing style of a vertex binding
    enum class VertexPacking { Interleaved, Separate };

    // Index type used by this mesh
    enum class IndexType : uint8_t { None = 0, UInt16, UInt32 };

    // Optional shader-contract check (name+type must exist)
    struct MeshContract
    {
        struct RequiredAttr { std::string name; ShaderDataType type; };
        std::vector<RequiredAttr> required;
    };

    // One CPU-side vertex buffer binding (binding index = position in vector)
    struct MeshDataBuffer
    {
        VertexPacking         packing = VertexPacking::Interleaved;
        std::vector<uint8_t> bytes;   // owned contiguous block
        GPUBufferLayout       layout;  // attribute descriptions + stride

        MeshDataBuffer() = default;
        MeshDataBuffer(MeshDataBuffer&&) noexcept = default;
        MeshDataBuffer& operator=(MeshDataBuffer&&) noexcept = default;

        uint32_t GetStride() const { return layout.GetStride(); }
        bool     Empty()     const { return bytes.empty(); }
        void     AdoptBytes(std::vector<uint8_t>&& src) noexcept { bytes = std::move(src); }
    };

    class MeshData final
    {
    public:
        // ---------------- Ctors / Moves (copy disabled) ----------------
        MeshData() = default;
        MeshData(const MeshData&) = delete;
        MeshData& operator=(const MeshData&) = delete;
        MeshData(MeshData&&) noexcept;
        MeshData& operator=(MeshData&&) noexcept;

        // ------------------------ Queries ------------------------------
        bool        IsIndexed() const;
        uint32_t    VertexCount() const            { return m_vertexCount; }
        uint32_t    IndexCount()  const            { return m_indexCount; }
        IndexType   GetIndexType() const           { return m_indexType; }
        GPUBufferUsageType Usage() const           { return m_usage; }
        void        SetUsage(GPUBufferUsageType u) { m_usage = u; }

        // Accessors for RHI upload (read-only)
        const std::vector<MeshDataBuffer>& GetMeshDataBuffers() const { return m_vbuffers; }
        const std::vector<uint16_t>& IndicesU16() const           { return m_indices16; }
        const std::vector<uint32_t>& IndicesU32() const           { return m_indices32; }

        // Attribute lookup / validation
        std::optional<std::pair<uint32_t,uint32_t>> FindAttribute(const std::string& name) const;
        bool HasAttribute(const std::string& name) const;
        bool Satisfies(const MeshContract& contract) const;

        // --------------- MOVE-FRIENDLY BUILDERS ----------------
        // Start a new interleaved binding (returns binding index)
        uint32_t BeginInterleavedBinding(GPUBufferUsageType usageHint = GPUBufferUsageType::STATIC);

        // Add an attribute to the interleaved binding BEFORE supplying bytes
        void AddInterleavedAttribute(uint32_t binding, ShaderDataType type,
                                     const std::string& name, bool normalized = false);

        // Adopt a fully built interleaved byte buffer (zero copy)
        // If vertexCount is 0, it will be derived from bytes/stride
        void AdoptInterleavedBytes(uint32_t binding, std::vector<uint8_t>&& bytesMove,
                                   uint32_t inVertexCount = 0);

        // One-shot: layout + bytes (move)
        uint32_t AddInterleavedBindingMove(GPUBufferLayout layout,
                                           std::vector<uint8_t>&& bytesMove,
                                           uint32_t inVertexCount = 0,
                                           GPUBufferUsageType usageHint = GPUBufferUsageType::STATIC);

        // Separate attribute: adopt bytes (one attribute per binding)
        uint32_t AddSeparateAttributeMove(ShaderDataType type, const std::string& name,
                                          std::vector<uint8_t>&& bytesMove,
                                          uint32_t inVertexCount,
                                          GPUBufferUsageType usageHint = GPUBufferUsageType::STATIC);

        // ------------------ Indices (move) -------------------
        void AdoptIndices(std::vector<uint16_t>&& idx,
                          GPUBufferUsageType usageHint = GPUBufferUsageType::STATIC) noexcept;

        void AdoptIndices(std::vector<uint32_t>&& idx,
                          GPUBufferUsageType usageHint = GPUBufferUsageType::STATIC) noexcept;

        // ------------- Legacy allocate/fill path -------------
        void AllocateInterleaved(uint32_t binding, uint32_t inVertexCount);

        template<typename T>
        void WriteInterleaved(uint32_t binding, uint32_t vertexIndex,
                              const std::string& name, const T& value)
        {
            auto& vb        = m_vbuffers[binding];
            const auto& els = vb.layout.GetElements();
            auto it = std::find_if(els.begin(), els.end(),
                                   [&](const GPUBufferElement& e){ return e.name == name; });
            if (it == els.end()) return;
            const uint32_t stride = vb.layout.GetStride();
            const uint32_t offset = it->offset;
            std::uint8_t* dst = vb.bytes.data()
                              + static_cast<size_t>(vertexIndex) * stride
                              + offset;
            std::memcpy(dst, &value, sizeof(T));
        }

        // ----------------- Post-upload cleanup ----------------
        void ClearMemory();

    private:
        // ------------ Indices ------------
        IndexType             m_indexType  = IndexType::None;
        uint32_t              m_indexCount = 0;
        std::vector<uint16_t> m_indices16;  // used if m_indexType == UInt16
        std::vector<uint32_t> m_indices32;  // used if m_indexType == UInt32

        // ------------ Vertices -----------
        uint32_t                     m_vertexCount = 0;
        std::vector<MeshDataBuffer> m_vbuffers;

        // ------------ Hints --------------
        GPUBufferUsageType m_usage = GPUBufferUsageType::STATIC;
    };

} // namespace AnalyticalApproach::Spellcore
