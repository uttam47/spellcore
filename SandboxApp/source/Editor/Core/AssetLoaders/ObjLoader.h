#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include <array>
#include <limits>

#include "core/MeshData.h"           
#include "core/GPUBufferLayout.h"     

namespace AnalyticalApproach::SpellcoreEditor
{
    // Thin, dependency-free OBJ loader that converts directly into Spellcore::MeshData.
    // Notes:
    //  * Scope: geometry only (v/vt/vn/f). Ignores mtllib/usemtl/groups/smoothing.
    //  * Triangulation: simple fan (v0, v(i-1), v(i)).
    //  * Indices: deduplicates (pos,uv,norm) tuples into a unified vertex stream.
    //  * Negative indices (OBJ spec) are supported: -1 means "last element" etc.
    class ObjLoader
    {
    public:
        // If uniqueVertexCount <= indexifyThreshold * expandedVertexCount => keep indexed, else expand.
        explicit ObjLoader(double indexifyThreshold = 0.9)
            : m_indexifyThreshold(indexifyThreshold) {}

        // Load from path into MeshData.
        //  - packing: Interleaved (single AoS binding) or Separate (one SoA binding per attribute).
        //  - forceIndexed: override heuristic. true=force indexed, false=force expanded, nullopt=use heuristic.
        AnalyticalApproach::Spellcore::MeshData*
        Load(const std::string& objPath,
            std::optional<bool> forceIndexed = std::nullopt,
            AnalyticalApproach::Spellcore::VertexPacking packing =
            AnalyticalApproach::Spellcore::VertexPacking::Interleaved) const;

    private:
        // ---------- Internal helpers ----------

        // Key for deduplication: indices into source (positions/texcoords/normals), 0-based or -1 if absent.
        struct FaceVertexKey
        {
            int p = -1; // position idx
            int t = -1; // texcoord idx
            int n = -1; // normal idx
            bool operator==(const FaceVertexKey& o) const { return p==o.p && t==o.t && n==o.n; }
        };
        struct FaceVertexKeyHash
        {
            size_t operator()(const FaceVertexKey& k) const noexcept
            {
                // Cheap 3-int hash with avalanche; fine for loader use.
                size_t h = static_cast<size_t>(k.p) ^ 0x9e3779b97f4a7c15ull;
                h ^= (static_cast<size_t>(k.t) + 0x9e3779b97f4a7c15ull + (h<<6) + (h>>2));
                h ^= (static_cast<size_t>(k.n) + 0x9e3779b97f4a7c15ull + (h<<6) + (h>>2));
                return h;
            }
        };

        // Trim ASCII whitespace at both ends (simple, locale-free).
        static void TrimInPlace(std::string& s);

        // s starts with prefix?
        static bool StartsWith(const std::string& s, const char* prefix);

        // Parse "v x y z" into out = {x,y,z}; returns false if malformed.
        static bool ParseFloat3(const std::string& line, std::array<float,3>& out);

        // Parse "vt u v" into out = {u,v}; returns false if malformed.
        static bool ParseFloat2(const std::string& line, std::array<float,2>& out);

        // Parse a face token "p/t/n", "p//n", "p/t" or "p" -> (p,t,n). 0 means "absent".
        static void ParseFaceTriplet(const std::string& tok, int& p, int& t, int& n);

        // Convert an OBJ index to zero-based: positive is 1-based, negative is from end, 0 means absent.
        static int ToZeroBased(int objIndex, int count);

        // Build a GPUBufferLayout for AoS (interleaved) with present attributes.
        static AnalyticalApproach::Spellcore::GPUBufferLayout
        MakeLayoutAoS(bool hasTex, bool hasNorm);

        // Append one interleaved vertex to dst (AoS). strideCached is computed once lazily.
        static void PackVertexAoS(std::vector<std::uint8_t>& dst,
                                  std::uint32_t& strideCached,
                                  const std::array<float,3>& pos,
                                  const std::optional<std::array<float,2>>& tex,
                                  const std::optional<std::array<float,3>>& nrm,
                                  bool hasTex, bool hasNorm);

        // Utility: true if value fits in 16-bit index.
        static bool FitsInU16(std::uint32_t v) { return v <= std::numeric_limits<std::uint16_t>::max(); }

    private:
        double m_indexifyThreshold; // Heuristic cutoff for keeping indexed geometry
    };
}
