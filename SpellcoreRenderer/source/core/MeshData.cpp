#include "MeshData.h"
#include <cassert>
#include <utility> // std::move

namespace AnalyticalApproach::Spellcore
{
	MeshData::MeshData(MeshData&& other) noexcept
		: m_indexType(other.m_indexType)
		, m_vbuffers(std::move(other.m_vbuffers))
		, m_indexCount(other.m_indexCount)
		, m_vertexCount(other.m_vertexCount)
		, m_usage(other.m_usage)
		, m_indices16(std::move(other.m_indices16))
		, m_indices32(std::move(other.m_indices32))
	{
		other.m_indexType = IndexType::None;
		other.m_indexCount = 0;
		other.m_vertexCount = 0;
		other.m_usage = GPUBufferUsageType::STATIC;
		other.m_indices16.clear();
		other.m_indices32.clear(); 
		other.m_vbuffers.clear();
	}

	MeshData& MeshData::operator=(MeshData&& other) noexcept
	{
		if (this != &other)
		{
			m_indexType = other.m_indexType;
			m_indexCount = other.m_indexCount;
			m_indices16 = std::move(other.m_indices16);
			m_indices32 = std::move(other.m_indices32);
			m_vertexCount = other.m_vertexCount;
			m_vbuffers = std::move(other.m_vbuffers);
			m_usage = other.m_usage;

			other.m_indexType = IndexType::None;
			other.m_indexCount = 0;
			other.m_vertexCount = 0;
			other.m_usage = GPUBufferUsageType::STATIC;
			other.m_indices16.clear();
			other.m_indices32.clear();
			other.m_vbuffers.clear();
		}
		return *this;
	}

	bool MeshData::IsIndexed() const
	{
		return (m_indexType == IndexType::UInt16 && !m_indices16.empty()) ||
			(m_indexType == IndexType::UInt32 && !m_indices32.empty());
	}

	std::optional<std::pair<uint32_t, uint32_t>>
		MeshData::FindAttribute(const std::string& name) const
	{
		for (uint32_t b = 0; b < m_vbuffers.size(); ++b)
		{
			const auto& elems = m_vbuffers[b].layout.GetElements();
			for (uint32_t e = 0; e < elems.size(); ++e)
				if (elems[e].name == name)
					return std::make_pair(b, e);
		}
		return std::nullopt;
	}

	bool MeshData::HasAttribute(const std::string& name) const
	{
		return FindAttribute(name).has_value();
	}

	bool MeshData::Satisfies(const MeshContract& contract) const
	{
		for (const auto& req : contract.required)
		{
			auto loc = FindAttribute(req.name);
			if (!loc) return false;
			const auto& elem = m_vbuffers[loc->first].layout.GetElements()[loc->second];
			if (elem.type != req.type) return false;
		}
		return true;
	}

	uint32_t MeshData::BeginInterleavedBinding(GPUBufferUsageType usageHint)
	{
		MeshDataBuffer vb;
		vb.packing = VertexPacking::Interleaved;
		vb.layout.gpuBufferType = GPUBufferType::VERTEX_BUFFER;
		vb.layout.gpuBufferSubType = GPUBufferSubType::VERTEX_DATA;
		vb.layout.gpuBufferUsageType = usageHint;
		m_vbuffers.push_back(std::move(vb));
		return static_cast<uint32_t>(m_vbuffers.size() - 1);
	}

	void MeshData::AddInterleavedAttribute(uint32_t binding, ShaderDataType type,
		const std::string& name, bool normalized)
	{
		auto elems = m_vbuffers[binding].layout.GetElements(); // copy current list
		elems.emplace_back(type, name, normalized);
		m_vbuffers[binding].layout = GPUBufferLayout{ elems };  // recompute offsets/stride
	}

	void MeshData::AdoptInterleavedBytes(uint32_t binding, std::vector<uint8_t>&& bytesMove,
		uint32_t inVertexCount)
	{
		auto& vb = m_vbuffers[binding];
		const uint32_t stride = vb.layout.GetStride();

		if (inVertexCount)
			m_vertexCount = inVertexCount;
		else
			m_vertexCount = (stride > 0) ? static_cast<uint32_t>(bytesMove.size() / stride) : 0;

		vb.AdoptBytes(std::move(bytesMove));
	}

	uint32_t MeshData::AddInterleavedBindingMove(GPUBufferLayout layout,
		std::vector<uint8_t>&& bytesMove,
		uint32_t inVertexCount,
		GPUBufferUsageType usageHint)
	{
		const uint32_t b = BeginInterleavedBinding(usageHint);
		m_vbuffers[b].layout = std::move(layout);
		AdoptInterleavedBytes(b, std::move(bytesMove), inVertexCount);
		return b;
	}

	uint32_t MeshData::AddSeparateAttributeMove(ShaderDataType type, const std::string& name,
		std::vector<uint8_t>&& bytesMove,
		uint32_t inVertexCount,
		GPUBufferUsageType usageHint)
	{
		MeshDataBuffer vb;
		vb.packing = VertexPacking::Separate;
		vb.layout.gpuBufferType = GPUBufferType::VERTEX_BUFFER;
		vb.layout.gpuBufferSubType = GPUBufferSubType::VERTEX_DATA;
		vb.layout.gpuBufferUsageType = usageHint;

		// single element, offset=0; stride = sizeof(type)
		std::vector<GPUBufferElement> elems;
		elems.emplace_back(type, name, /*normalized*/false);
		vb.layout = GPUBufferLayout{ elems };

		const uint32_t stride = vb.layout.GetStride();
		const size_t   needed = static_cast<size_t>(inVertexCount) * stride;
		if (bytesMove.size() != needed)
		{
			// derive vertex count if possible; otherwise leave as-is (caller can validate)
			inVertexCount = (stride ? static_cast<uint32_t>(bytesMove.size() / stride) : 0);
		}

		vb.bytes = std::move(bytesMove);
		m_vbuffers.push_back(std::move(vb));

		if (m_vertexCount == 0) m_vertexCount = inVertexCount;
		// (optional) assert(m_vertexCount == inVertexCount);

		return static_cast<uint32_t>(m_vbuffers.size() - 1);
	}

	void MeshData::AdoptIndices(std::vector<uint16_t>&& idx,
		GPUBufferUsageType usageHint) noexcept
	{
		m_indexType = IndexType::UInt16;
		m_indexCount = static_cast<uint32_t>(idx.size());
		m_indices16 = std::move(idx);
		m_usage = usageHint;
		m_indices32.clear();
		m_indices32.shrink_to_fit();
	}

	void MeshData::AdoptIndices(std::vector<uint32_t>&& idx,
		GPUBufferUsageType usageHint) noexcept
	{
		m_indexType = IndexType::UInt32;
		m_indexCount = static_cast<uint32_t>(idx.size());
		m_indices32 = std::move(idx);
		m_usage = usageHint;
		m_indices16.clear();
		m_indices16.shrink_to_fit();
	}

	void MeshData::AllocateInterleaved(uint32_t binding, uint32_t inVertexCount)
	{
		m_vertexCount = inVertexCount;
		auto& vb = m_vbuffers[binding];
		const uint32_t stride = vb.layout.GetStride();
		vb.bytes.resize(static_cast<size_t>(stride) * m_vertexCount);
	}

	void MeshData::ClearMemory()
	{
		m_indices16.clear(); m_indices16.shrink_to_fit();
		m_indices32.clear(); m_indices32.shrink_to_fit();
		for (auto& vb : m_vbuffers)
		{
			vb.bytes.clear();
			vb.bytes.shrink_to_fit();
		}
	}

} // namespace AnalyticalApproach::Spellcore
