#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include <array>

#include "core/GPUBufferLayout.h"
#include "FileUtils/FileReader.h"

namespace AnalyticalApproach::SpellcoreEditor
{
    enum class VertexPacking { Interleaved, Separate };

    // CPU-side mesh buffers (ready to upload)
    struct LoadedMesh
    {
        // ----- Common -----
        bool         isIndexed    = false;
        std::uint32_t vertexCount = 0;
        std::uint32_t indexCount  = 0;
        std::vector<std::uint32_t> indexBuffer;

        // Raw attribute presence (as found in OBJ)
        bool hasTexcoord = false;
        bool hasNormal   = false;

        // Chosen packing:
        VertexPacking packing = VertexPacking::Interleaved;

        // ----- Interleaved (AoS) -----
        // Valid when packing == Interleaved
        std::vector<std::uint8_t>                 vertexBuffer;   // interleaved VB
        std::uint32_t                             vertexStride = 0;
        AnalyticalApproach::Spellcore::GPUBufferLayout layout;   // AoS layout

        // ----- Separate (SoA) -----
        // Valid when packing == Separate
        // Flat arrays: size = vertexCount * components
        std::vector<float> positionsSoA; // 3*vertexCount
        std::vector<float> normalsSoA;   // 3*vertexCount (if hasNormal)
        std::vector<float> texcoordsSoA; // 2*vertexCount (if hasTexcoord)

        // Per-buffer layouts (convenience)
        std::optional<AnalyticalApproach::Spellcore::GPUBufferLayout> layoutPos;
        std::optional<AnalyticalApproach::Spellcore::GPUBufferLayout> layoutNrm;
        std::optional<AnalyticalApproach::Spellcore::GPUBufferLayout> layoutUv;
    };

    // Lightweight OBJ loader (v/vt/vn; triangulates ngons; supports negative indices)
    // Deduplicates (pos,uv,norm) tuples to unify GPU indexing or expands to non-indexed.
    class ObjAssetLoader
    {
    public:
        // If uniqueVertexCount <= indexifyThreshold * expandedVertexCount => keep indexed, else expand
        explicit ObjAssetLoader(double indexifyThreshold = 0.9)
            : _indexifyThreshold(indexifyThreshold) {}

        // forceIndexed: override heuristic
        // packing: Interleaved (default) or Separate buffers
        LoadedMesh Load(const std::string& objPath,
                        std::optional<bool> forceIndexed = std::nullopt,
                        VertexPacking packing = VertexPacking::Interleaved) const;

    private:
        struct FaceVertexKey
        {
            int p = -1; // position idx (0-based stored; OBJ is 1-based)
            int t = -1; // texcoord idx
            int n = -1; // normal idx

            bool operator==(const FaceVertexKey& other) const
            { return p == other.p && t == other.t && n == other.n; }
        };

        struct FaceVertexKeyHash
        {
            size_t operator()(const FaceVertexKey& k) const noexcept
            {
                size_t h = static_cast<size_t>(k.p + 0x9e3779b9);
                h ^= static_cast<size_t>(k.t + 0x9e3779b9) + (h << 6) + (h >> 2);
                h ^= static_cast<size_t>(k.n + 0x9e3779b9) + (h << 6) + (h >> 2);
                return h;
            }
        };

        // Layout builders
        AnalyticalApproach::Spellcore::GPUBufferLayout MakeLayoutAoS(bool hasTex, bool hasNorm) const;
        AnalyticalApproach::Spellcore::GPUBufferLayout MakePosLayout() const;
        AnalyticalApproach::Spellcore::GPUBufferLayout MakeNrmLayout() const;
        AnalyticalApproach::Spellcore::GPUBufferLayout MakeUvLayout()  const;

        // Interleaved vertex packer
        static void PackVertexAoS(std::vector<std::uint8_t>& dst,
                                  std::uint32_t& strideCached,
                                  const std::array<float,3>& pos,
                                  const std::optional<std::array<float,2>>& tex,
                                  const std::optional<std::array<float,3>>& nrm,
                                  bool hasTex, bool hasNorm);

        // Parse helpers
        static bool ParseFloat3(const std::string& line, std::array<float,3>& out);
        static bool ParseFloat2(const std::string& line, std::array<float,2>& out);
        static void ParseFaceTriplet(const std::string& tok, int& p, int& t, int& n);

    private:
        double _indexifyThreshold;
    };
}
