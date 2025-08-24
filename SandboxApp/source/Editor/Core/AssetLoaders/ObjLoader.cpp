#include "ObjLoader.h"
#include <sstream>
#include <string>
#include <charconv>
#include <limits>
#include <cstdlib>
#include <cstring> // memcpy

using namespace AnalyticalApproach::Spellcore;

namespace AnalyticalApproach::SpellcoreEditor
{
    // -----------------------
    // Small parsing utilities
    // -----------------------
    static inline void trim_inplace(std::string& s)
    {
        auto notspace = [](int ch){ return !(ch==' ' || ch=='\t' || ch=='\r' || ch=='\n'); };
        size_t b = 0; while (b < s.size() && !notspace(s[b])) ++b;
        size_t e = s.size(); while (e>b && !notspace(s[e-1])) --e;
        if (b || e < s.size()) s = s.substr(b, e-b);
    }
    static inline bool starts_with(const std::string& s, const char* prefix)
    {
        const size_t n = std::strlen(prefix);
        return s.size() >= n && std::equal(prefix, prefix+n, s.begin());
    }

    bool ObjLoader::ParseFloat3(const std::string& line, std::array<float,3>& out)
    {
        std::istringstream iss(line);
        std::string tag; float x,y,z;
        if (!(iss >> tag >> x >> y >> z)) return false;
        out = {x,y,z};
        return true;
    }
    bool ObjLoader::ParseFloat2(const std::string& line, std::array<float,2>& out)
    {
        std::istringstream iss(line);
        std::string tag; float u,v;
        if (!(iss >> tag >> u >> v)) return false;
        out = {u,v};
        return true;
    }

    void ObjLoader::ParseFaceTriplet(const std::string& tok, int& p, int& t, int& n)
    {
        // OBJ 1-based; negative allowed; 0/empty means "not present"
        p = t = n = 0;
        size_t s1 = tok.find('/');
        if (s1 == std::string::npos) { p = std::atoi(tok.c_str()); return; }
        if (s1 > 0) p = std::atoi(tok.substr(0, s1).c_str());
        size_t s2 = tok.find('/', s1 + 1);
        if (s2 == std::string::npos) { if (s1 + 1 < tok.size()) t = std::atoi(tok.substr(s1 + 1).c_str()); return; }
        if (s2 == s1 + 1) { if (s2 + 1 < tok.size()) n = std::atoi(tok.substr(s2 + 1).c_str()); } // v//vn
        else { t = std::atoi(tok.substr(s1 + 1, s2 - (s1 + 1)).c_str()); if (s2 + 1 < tok.size()) n = std::atoi(tok.substr(s2 + 1).c_str()); }
    }

    // -----------------------------------
    // Layout builders
    // -----------------------------------
    GPUBufferLayout ObjLoader::MakeLayoutAoS(bool hasTex, bool hasNorm) const
    {
        std::vector<GPUBufferElement> elems;
        elems.emplace_back(ShaderDataType::Float3, "a_Position", false);
        if (hasTex)  elems.emplace_back(ShaderDataType::Float2, "a_TexCoord", false);
        if (hasNorm) elems.emplace_back(ShaderDataType::Float3, "a_Normal",   false);

        GPUBufferLayout layout(elems);
        layout.gpuBufferType      = GPUBufferType::VERTEX_BUFFER;
        layout.gpuBufferSubType   = GPUBufferSubType::VERTEX_DATA;
        layout.gpuBufferUsageType = GPUBufferUsageType::STATIC;
        return layout;
    }

    GPUBufferLayout ObjLoader::MakePosLayout() const
    {
        std::vector<GPUBufferElement> elems;
        elems.emplace_back(ShaderDataType::Float3, "a_Position", false);
        GPUBufferLayout layout(elems);
        layout.gpuBufferType      = GPUBufferType::VERTEX_BUFFER;
        layout.gpuBufferSubType   = GPUBufferSubType::VERTEX_DATA;
        layout.gpuBufferUsageType = GPUBufferUsageType::STATIC;
        return layout;
    }
    GPUBufferLayout ObjLoader::MakeNrmLayout() const
    {
        std::vector<GPUBufferElement> elems;
        elems.emplace_back(ShaderDataType::Float3, "a_Normal", false);
        GPUBufferLayout layout(elems);
        layout.gpuBufferType      = GPUBufferType::VERTEX_BUFFER;
        layout.gpuBufferSubType   = GPUBufferSubType::VERTEX_DATA;
        layout.gpuBufferUsageType = GPUBufferUsageType::STATIC;
        return layout;
    }
    GPUBufferLayout ObjLoader::MakeUvLayout() const
    {
        std::vector<GPUBufferElement> elems;
        elems.emplace_back(ShaderDataType::Float2, "a_TexCoord", false);
        GPUBufferLayout layout(elems);
        layout.gpuBufferType      = GPUBufferType::VERTEX_BUFFER;
        layout.gpuBufferSubType   = GPUBufferSubType::VERTEX_DATA;
        layout.gpuBufferUsageType = GPUBufferUsageType::STATIC;
        return layout;
    }

    // -----------------------------------
    // AoS packing
    // -----------------------------------
    void ObjLoader::PackVertexAoS(std::vector<std::uint8_t>& dst,
                                       std::uint32_t& strideCached,
                                       const std::array<float,3>& pos,
                                       const std::optional<std::array<float,2>>& tex,
                                       const std::optional<std::array<float,3>>& nrm,
                                       bool hasTex, bool hasNorm)
    {
        if (strideCached == 0)
        {
            strideCached = sizeof(float)*3
                         + (hasTex  ? sizeof(float)*2 : 0)
                         + (hasNorm ? sizeof(float)*3 : 0);
        }
        const size_t oldSize = dst.size();
        dst.resize(oldSize + strideCached);
        std::uint8_t* write = dst.data() + oldSize;

        std::memcpy(write, pos.data(), sizeof(float)*3);
        write += sizeof(float)*3;

        if (hasTex)
        {
            const float uv[2] = { tex ? (*tex)[0] : 0.f, tex ? (*tex)[1] : 0.f };
            std::memcpy(write, uv, sizeof(float)*2);
            write += sizeof(float)*2;
        }
        if (hasNorm)
        {
            const float nn[3] = { nrm ? (*nrm)[0] : 0.f,
                                  nrm ? (*nrm)[1] : 0.f,
                                  nrm ? (*nrm)[2] : 1.f };
            std::memcpy(write, nn, sizeof(float)*3);
            write += sizeof(float)*3;
        }
    }

    // -----------
    // Main entry
    // -----------
    MeshData ObjLoader::Load(const std::string& objPath,
                                    std::optional<bool> forceIndexed,
                                    VertexPacking packing) const
    {
        MeshData out;
        out.packing = packing;

        // Read file
        AnalyticalApproach::FileSystem::FileReader fr(objPath);
        std::string content = fr.getContent();
        if (content.empty())
            return out;

        // Parse attribute streams
        std::vector<std::array<float,3>> positions;
        std::vector<std::array<float,2>> texcoords;
        std::vector<std::array<float,3>> normals;

        struct FaceTriplet { int p, t, n; };
        std::vector<FaceTriplet> faceTriplets; // triangulated

        positions.reserve(1024);
        texcoords.reserve(1024);
        normals.reserve(1024);
        faceTriplets.reserve(3072);

        std::istringstream iss(content);
        std::string line;
        while (std::getline(iss, line))
        {
            trim_inplace(line);
            if (line.empty() || line[0] == '#') continue;

            if (starts_with(line, "v "))
            {
                std::array<float,3> v;
                if (ParseFloat3(line, v)) positions.push_back(v);
            }
            else if (starts_with(line, "vt "))
            {
                std::array<float,2> vt;
                if (ParseFloat2(line, vt)) texcoords.push_back(vt);
            }
            else if (starts_with(line, "vn "))
            {
                std::array<float,3> vn;
                if (ParseFloat3(line, vn)) normals.push_back(vn);
            }
            else if (starts_with(line, "f "))
            {
                std::istringstream fl(line.substr(2));
                std::vector<std::string> toks; std::string tok;
                while (fl >> tok) toks.push_back(tok);
                if (toks.size() < 3) continue;

                auto parseTok = [&](const std::string& s)->FaceTriplet{
                    int p=0,t=0,n=0; ParseFaceTriplet(s, p,t,n); return FaceTriplet{p,t,n};
                };

                const FaceTriplet v0 = parseTok(toks[0]);
                for (size_t i = 2; i < toks.size(); ++i)
                {
                    FaceTriplet v1 = parseTok(toks[i - 1]);
                    FaceTriplet v2 = parseTok(toks[i]);
                    faceTriplets.push_back(v0);
                    faceTriplets.push_back(v1);
                    faceTriplets.push_back(v2);
                }
            }
        }

        // Detect attribute presence
        bool hasTex = false, hasNorm = false;
        for (const auto& ft : faceTriplets)
        {
            if (ft.t != 0) hasTex  = true;
            if (ft.n != 0) hasNorm = true;
            if (hasTex && hasNorm) break;
        }
        out.hasTexcoord = hasTex;
        out.hasNormal   = hasNorm;

        // Helpers
        auto toZeroBased = [](int idx, int count)->int {
            if (idx > 0) return idx - 1;          // OBJ 1-based
            if (idx < 0) return count + idx;      // negative => from end
            return -1;                             // 0 or invalid
        };

        // Unified-vertex map (pos,uv,nrm)
        std::unordered_map<FaceVertexKey, std::uint32_t, FaceVertexKeyHash> uniqueMap;
        uniqueMap.reserve(faceTriplets.size());

        std::vector<std::uint32_t> indices;
        indices.reserve(faceTriplets.size());

        // Prepare outputs depending on packing
        std::vector<std::uint8_t> interleaved;  // AoS temp
        std::uint32_t strideCached = 0;

        std::vector<float> posSoA; posSoA.reserve(faceTriplets.size()*3);
        std::vector<float> nrmSoA; if (hasNorm) nrmSoA.reserve(faceTriplets.size()*3);
        std::vector<float> uvSoA;  if (hasTex)  uvSoA.reserve(faceTriplets.size()*2);

        // Build unique vertices
        auto pushUniqueAos = [&](const std::array<float,3>& pos,
                                 const std::optional<std::array<float,2>>& uv,
                                 const std::optional<std::array<float,3>>& nrm)->std::uint32_t
        {
            PackVertexAoS(interleaved, strideCached, pos, uv, nrm, hasTex, hasNorm);
            return static_cast<std::uint32_t>(interleaved.size() / (strideCached ? strideCached : 1) - 1);
        };

        auto pushUniqueSoA = [&](const std::array<float,3>& pos,
                                 const std::optional<std::array<float,2>>& uv,
                                 const std::optional<std::array<float,3>>& nrm)->std::uint32_t
        {
            // append position
            posSoA.insert(posSoA.end(), {pos[0], pos[1], pos[2]});
            // append uv/nrm if present (fill zeros otherwise only if attribute exists)
            if (hasTex)
            {
                const float u = uv ? (*uv)[0] : 0.f;
                const float v = uv ? (*uv)[1] : 0.f;
                uvSoA.insert(uvSoA.end(), {u, v});
            }
            if (hasNorm)
            {
                const float nx = nrm ? (*nrm)[0] : 0.f;
                const float ny = nrm ? (*nrm)[1] : 0.f;
                const float nz = nrm ? (*nrm)[2] : 1.f;
                nrmSoA.insert(nrmSoA.end(), {nx, ny, nz});
            }
            // index = new vertex id
            const std::uint32_t newIdx = static_cast<std::uint32_t>(posSoA.size()/3 - 1);
            return newIdx;
        };

        for (const auto& ft : faceTriplets)
        {
            FaceVertexKey key{
                toZeroBased(ft.p, static_cast<int>(positions.size())),
                toZeroBased(ft.t, static_cast<int>(texcoords.size())),
                toZeroBased(ft.n, static_cast<int>(normals.size()))
            };

            auto it = uniqueMap.find(key);
            if (it == uniqueMap.end())
            {
                // Fetch attribute values
                std::array<float,3> pos = (key.p >= 0 && key.p < (int)positions.size())
                    ? positions[key.p] : std::array<float,3>{0,0,0};

                std::optional<std::array<float,2>> uv;
                if (hasTex && key.t >= 0 && key.t < (int)texcoords.size())
                    uv = texcoords[key.t];

                std::optional<std::array<float,3>> nrm;
                if (hasNorm && key.n >= 0 && key.n < (int)normals.size())
                    nrm = normals[key.n];

                std::uint32_t newIndex =
                    (packing == VertexPacking::Interleaved)
                    ? pushUniqueAos(pos, uv, nrm)
                    : pushUniqueSoA(pos, uv, nrm);

                uniqueMap.emplace(key, newIndex);
                indices.push_back(newIndex);
            }
            else
            {
                indices.push_back(it->second);
            }
        }

        // Decide indexed vs non-indexed
        std::uint32_t uniqueVertexCount =
            (packing == VertexPacking::Interleaved)
            ? static_cast<std::uint32_t>(interleaved.empty() ? 0u : interleaved.size() / strideCached)
            : static_cast<std::uint32_t>(posSoA.size() / 3);

        const std::uint32_t expandedVertexCount =
            static_cast<std::uint32_t>(faceTriplets.size());

        const bool useIndexed = forceIndexed.value_or(
            uniqueVertexCount <= static_cast<std::uint32_t>(_indexifyThreshold * expandedVertexCount)
        );

        // Fill out struct
        out.isIndexed = useIndexed;

        if (packing == VertexPacking::Interleaved)
        {
            out.layout       = MakeLayoutAoS(hasTex, hasNorm);
            out.vertexStride = out.layout.GetStride();

            if (useIndexed)
            {
                out.vertexBuffer = std::move(interleaved);
                out.indexBuffer  = std::move(indices);
                out.vertexCount  = uniqueVertexCount;
                out.indexCount   = static_cast<std::uint32_t>(out.indexBuffer.size());
            }
            else
            {
                // expand interleaved in triangle order
                std::vector<std::uint8_t> expandedVB;
                expandedVB.reserve(static_cast<size_t>(expandedVertexCount) * out.vertexStride);
                auto writeVertexByIndex = [&](std::uint32_t idx)
                {
                    const size_t srcOff = static_cast<size_t>(idx) * out.vertexStride;
                    expandedVB.insert(expandedVB.end(),
                                      interleaved.begin() + srcOff,
                                      interleaved.begin() + srcOff + out.vertexStride);
                };
                for (std::uint32_t idx : indices) writeVertexByIndex(idx);

                out.vertexBuffer = std::move(expandedVB);
                out.indexBuffer.clear();
                out.vertexCount = expandedVertexCount;
                out.indexCount  = 0;
            }
        }
        else // Separate (SoA)
        {
            // Build per-buffer layouts
            out.layoutPos = MakePosLayout();
            if (hasNorm) out.layoutNrm = MakeNrmLayout();
            if (hasTex)  out.layoutUv  = MakeUvLayout();

            if (useIndexed)
            {
                out.positionsSoA = std::move(posSoA);
                if (hasNorm) out.normalsSoA   = std::move(nrmSoA);
                if (hasTex)  out.texcoordsSoA = std::move(uvSoA);

                out.indexBuffer = std::move(indices);
                out.vertexCount = uniqueVertexCount;
                out.indexCount  = static_cast<std::uint32_t>(out.indexBuffer.size());
            }
            else
            {
                // Expand SoA arrays in triangle order
                std::vector<float> posExp; posExp.reserve(expandedVertexCount*3);
                std::vector<float> nrmExp; if (hasNorm) nrmExp.reserve(expandedVertexCount*3);
                std::vector<float> uvExp;  if (hasTex)  uvExp.reserve(expandedVertexCount*2);

                auto copyByIndex = [&](std::uint32_t idx)
                {
                    // positions
                    posExp.insert(posExp.end(),
                                  { posSoA[idx*3+0], posSoA[idx*3+1], posSoA[idx*3+2] });
                    if (hasNorm)
                        nrmExp.insert(nrmExp.end(),
                                      { nrmSoA[idx*3+0], nrmSoA[idx*3+1], nrmSoA[idx*3+2] });
                    if (hasTex)
                        uvExp.insert(uvExp.end(),
                                     { uvSoA[idx*2+0], uvSoA[idx*2+1] });
                };
                for (std::uint32_t idx : indices) copyByIndex(idx);

                out.positionsSoA = std::move(posExp);
                if (hasNorm) out.normalsSoA   = std::move(nrmExp);
                if (hasTex)  out.texcoordsSoA = std::move(uvExp);

                out.indexBuffer.clear();
                out.vertexCount = expandedVertexCount;
                out.indexCount  = 0;
            }
        }

        return out;
    }
}
