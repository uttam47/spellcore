#include "ObjLoader.h"

#include <fstream>
#include <sstream>
#include <cstring>   // std::memcpy
#include <optional>

using namespace AnalyticalApproach::Spellcore;

namespace AnalyticalApproach::SpellcoreEditor
{
    // ------------------------------
    // Tiny, dependency-free helpers
    // ------------------------------
    void ObjLoader::TrimInPlace(std::string& s)
    {
        auto notspace = [](unsigned char c){ return !(c==' '||c=='\t'||c=='\r'||c=='\n'); };
        size_t b=0; while (b<s.size() && !notspace((unsigned char)s[b])) ++b;
        size_t e=s.size(); while (e>b && !notspace((unsigned char)s[e-1])) --e;
        if (b || e<s.size()) s = s.substr(b, e-b);
    }
    bool ObjLoader::StartsWith(const std::string& s, const char* prefix)
    {
        const size_t n = std::strlen(prefix);
        return s.size() >= n && std::memcmp(s.data(), prefix, n) == 0;
    }
    bool ObjLoader::ParseFloat3(const std::string& line, std::array<float,3>& out)
    {
        // Accepts:  v  x y z   or   vn x y z
        std::istringstream iss(line);
        std::string tag; float x,y,z;
        if (!(iss >> tag >> x >> y >> z)) return false;
        out = {x,y,z};
        return true;
    }
    bool ObjLoader::ParseFloat2(const std::string& line, std::array<float,2>& out)
    {
        // Accepts:  vt u v
        std::istringstream iss(line);
        std::string tag; float u,v;
        if (!(iss >> tag >> u >> v)) return false;
        out = {u,v};
        return true;
    }
    void ObjLoader::ParseFaceTriplet(const std::string& tok, int& p, int& t, int& n)
    {
        // Handles: "p", "p/t", "p//n", "p/t/n"
        p = t = n = 0;
        size_t s1 = tok.find('/');
        if (s1 == std::string::npos) { p = std::atoi(tok.c_str()); return; }
        if (s1 > 0) p = std::atoi(tok.substr(0, s1).c_str());
        size_t s2 = tok.find('/', s1 + 1);
        if (s2 == std::string::npos) { if (s1 + 1 < tok.size()) t = std::atoi(tok.substr(s1 + 1).c_str()); return; }
        if (s2 == s1 + 1) { if (s2 + 1 < tok.size()) n = std::atoi(tok.substr(s2 + 1).c_str()); } // p//n
        else { t = std::atoi(tok.substr(s1 + 1, s2 - (s1 + 1)).c_str()); if (s2 + 1 < tok.size()) n = std::atoi(tok.substr(s2 + 1).c_str()); }
    }
    int ObjLoader::ToZeroBased(int objIndex, int count)
    {
        if (objIndex > 0) return objIndex - 1;     // OBJ positive: 1..N
        if (objIndex < 0) return count + objIndex; // OBJ negative: -1 = last
        return -1;                                  // 0 or missing => absent
    }

    // ------------------------------
    // AoS layout + packing helpers
    // ------------------------------
    GPUBufferLayout ObjLoader::MakeLayoutAoS(bool hasTex, bool hasNorm)
    {
        // Build an interleaved layout that matches the attributes we actually have:
        //   position (always present) + optional texcoord + optional normal
        std::vector<GPUBufferElement> elems;
        elems.emplace_back(ShaderDataType::Float3, "a_Position", false);
        if (hasTex)  elems.emplace_back(ShaderDataType::Float2, "a_TexCoord", false);
        if (hasNorm) elems.emplace_back(ShaderDataType::Float3, "a_Normal",   false);

        GPUBufferLayout layout(elems);
        layout.gpuBufferType       = GPUBufferType::VERTEX_BUFFER;
        layout.gpuBufferSubType    = GPUBufferSubType::VERTEX_DATA;
        layout.gpuBufferUsageType  = GPUBufferUsageType::STATIC;
        return layout;
    }

    void ObjLoader::PackVertexAoS(std::vector<std::uint8_t>& dst,
                                  std::uint32_t& strideCached,
                                  const std::array<float,3>& pos,
                                  const std::optional<std::array<float,2>>& tex,
                                  const std::optional<std::array<float,3>>& nrm,
                                  bool hasTex, bool hasNorm)
    {
        // Lazily compute the stride once (3 + [2] + [3]) * sizeof(float).
        if (strideCached == 0)
        {
            strideCached = sizeof(float) * (3 + (hasTex?2:0) + (hasNorm?3:0));
        }

        const size_t oldSize = dst.size();
        dst.resize(oldSize + strideCached);
        std::uint8_t* write = dst.data() + oldSize;

        // Position (xyz)
        std::memcpy(write, pos.data(), sizeof(float)*3);
        write += sizeof(float)*3;

        // TexCoord (uv) — fill 0 if attribute exists but current vertex lacks vt.
        if (hasTex)
        {
            const float uv[2] = { tex ? (*tex)[0] : 0.f, tex ? (*tex)[1] : 0.f };
            std::memcpy(write, uv, sizeof(float)*2);
            write += sizeof(float)*2;
        }

        // Normal (xyz) — default to +Z if missing.
        if (hasNorm)
        {
            const float nn[3] = { nrm ? (*nrm)[0] : 0.f,
                                  nrm ? (*nrm)[1] : 0.f,
                                  nrm ? (*nrm)[2] : 1.f };
            std::memcpy(write, nn, sizeof(float)*3);
            // write += sizeof(float)*3; // not used after this
        }
    }

    // ------------------------------
    //              Load
    // ------------------------------
    MeshData* ObjLoader::Load(const std::string& objPath,
                             std::optional<bool> forceIndexed,
                             VertexPacking packing) const
    {
        MeshData* out = new MeshData();                    // final product (move-only, big buffers adopted)
        out->SetUsage(GPUBufferUsageType::STATIC);

        // ---- 1) Slurp file into memory (simple text read) ----
        std::ifstream in(objPath, std::ios::in | std::ios::binary);
        if (!in) return out;            // return empty mesh if file not found

        std::string content;
        in.seekg(0, std::ios::end);
        content.resize(static_cast<size_t>(in.tellg()));
        in.seekg(0, std::ios::beg);
        in.read(content.data(), static_cast<std::streamsize>(content.size()));

        // ---- 2) Parse OBJ streams: v / vt / vn / f ----
        std::vector<std::array<float,3>> positions; positions.reserve(4096);
        std::vector<std::array<float,2>> texcoords; texcoords.reserve(4096);
        std::vector<std::array<float,3>> normals;   normals.reserve(4096);

        // We’ll collect faces as a flat list of triplets (already triangulated).
        struct Triplet { int p, t, n; };
        std::vector<Triplet> faceVerts; faceVerts.reserve(8192);

        std::istringstream iss(content);
        std::string line;
        while (std::getline(iss, line))
        {
            TrimInPlace(line);
            if (line.empty() || line[0] == '#') continue;

            if (StartsWith(line, "v "))
            {
                std::array<float,3> v; if (ParseFloat3(line, v)) positions.push_back(v);
            }
            else if (StartsWith(line, "vt "))
            {
                std::array<float,2> vt; if (ParseFloat2(line, vt)) texcoords.push_back(vt);
            }
            else if (StartsWith(line, "vn "))
            {
                std::array<float,3> vn; if (ParseFloat3(line, vn)) normals.push_back(vn);
            }
            else if (StartsWith(line, "f "))
            {
                // Tokenize face line (after the "f ")
                std::istringstream fl(line.substr(2));
                std::vector<std::string> toks; toks.reserve(8);
                std::string tok;
                while (fl >> tok) toks.push_back(tok);
                if (toks.size() < 3) continue; // not enough to make a triangle

                // Convert tokens to Triplet (OBJ indices may be p, p/t, p//n, p/t/n)
                auto parseTok = [&](const std::string& s)->Triplet {
                    int p=0,t=0,n=0; ParseFaceTriplet(s, p,t,n); return {p,t,n};
                };

                // Fan triangulation: (v0, v(i-1), v(i))
                const Triplet v0 = parseTok(toks[0]);
                for (size_t i = 2; i < toks.size(); ++i)
                {
                    Triplet v1 = parseTok(toks[i-1]);
                    Triplet v2 = parseTok(toks[i]);
                    faceVerts.push_back(v0);
                    faceVerts.push_back(v1);
                    faceVerts.push_back(v2);
                }
            }
        }

        // ---- 3) Detect attribute presence from the faces we actually use ----
        bool hasTex = false, hasNorm = false;
        for (const auto& ft : faceVerts)
        {
            if (ft.t != 0) hasTex  = true;
            if (ft.n != 0) hasNorm = true;
            if (hasTex && hasNorm) break;
        }

        // ---- 4) Deduplicate (pos, uv, norm) tuples into unified vertices ----
        std::unordered_map<FaceVertexKey, std::uint32_t, FaceVertexKeyHash> unique;
        unique.reserve(faceVerts.size());

        std::vector<std::uint32_t> indices;          // will hold remapped triangle indices
        indices.reserve(faceVerts.size());

        // Temp containers for building vertex data
        std::vector<std::uint8_t> interleaved;       // AoS path
        std::uint32_t strideCached = 0;              // computed lazily once for AoS

        std::vector<float> posSoA; posSoA.reserve(faceVerts.size()*3);
        std::vector<float> nrmSoA; if (hasNorm) nrmSoA.reserve(faceVerts.size()*3);
        std::vector<float> uvSoA;  if (hasTex)  uvSoA.reserve(faceVerts.size()*2);

        // Append a brand-new vertex (AoS)
        auto pushAos = [&](const std::array<float,3>& pos,
                           const std::optional<std::array<float,2>>& uv,
                           const std::optional<std::array<float,3>>& nrm) -> std::uint32_t
        {
            PackVertexAoS(interleaved, strideCached, pos, uv, nrm, hasTex, hasNorm);
            // (current vertex index) = (new size / stride) - 1
            const std::uint32_t idx = strideCached ? (static_cast<std::uint32_t>(interleaved.size()/strideCached) - 1) : 0u;
            return idx;
        };

        // Append a brand-new vertex (SoA)
        auto pushSoa = [&](const std::array<float,3>& pos,
                           const std::optional<std::array<float,2>>& uv,
                           const std::optional<std::array<float,3>>& nrm) -> std::uint32_t
        {
            posSoA.insert(posSoA.end(), {pos[0], pos[1], pos[2]});
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
            // return new vertex index (3 floats per pos)
            return static_cast<std::uint32_t>(posSoA.size()/3 - 1);
        };

        // Build unified vertices by walking face triplets
        for (const auto& ft : faceVerts)
        {
            FaceVertexKey key{
                ToZeroBased(ft.p, static_cast<int>(positions.size())),
                ToZeroBased(ft.t, static_cast<int>(texcoords.size())),
                ToZeroBased(ft.n, static_cast<int>(normals.size()))
            };

            auto it = unique.find(key);
            if (it == unique.end())
            {
                // Fetch source attribs (defaults if missing)
                std::array<float,3> pos = (key.p >= 0 && key.p < (int)positions.size())
                                           ? positions[key.p] : std::array<float,3>{0,0,0};

                std::optional<std::array<float,2>> uv;
                if (hasTex && key.t >= 0 && key.t < (int)texcoords.size())
                    uv = texcoords[key.t];

                std::optional<std::array<float,3>> nrm;
                if (hasNorm && key.n >= 0 && key.n < (int)normals.size())
                    nrm = normals[key.n];

                std::uint32_t newIdx =
                    (packing == VertexPacking::Interleaved) ? pushAos(pos, uv, nrm)
                                                            : pushSoa(pos, uv, nrm);

                unique.emplace(key, newIdx);
                indices.push_back(newIdx);
            }
            else
            {
                indices.push_back(it->second);
            }
        }

        // ---- 5) Choose indexed vs expanded (heuristic or forced) ----
        const std::uint32_t uniqueCount =
            (packing == VertexPacking::Interleaved)
                ? (strideCached ? static_cast<std::uint32_t>(interleaved.size()/strideCached) : 0u)
                : static_cast<std::uint32_t>(posSoA.size()/3);

        const std::uint32_t expandedCount = static_cast<std::uint32_t>(faceVerts.size());

        const bool keepIndexed = forceIndexed.value_or(
            uniqueCount <= static_cast<std::uint32_t>(m_indexifyThreshold * expandedCount)
        );

        // ---- 6) Build MeshData (adopt big buffers by move) ----
        if (packing == VertexPacking::Interleaved)
        {
            // Interleaved: single binding with POSITION + [TEXCOORD] + [NORMAL].
            GPUBufferLayout layout = MakeLayoutAoS(hasTex, hasNorm);

            if (keepIndexed)
            {
                // (a) Indexed: adopt AoS as-is + adopt indices
                out->AddInterleavedBindingMove(std::move(layout), std::move(interleaved), uniqueCount);

                // Prefer u16 indices if it fits (smaller/faster).
                if (FitsInU16(uniqueCount))
                {
                    std::vector<uint16_t> idx16; idx16.reserve(indices.size());
                    for (uint32_t v : indices) idx16.push_back(static_cast<uint16_t>(v));
                    out->AdoptIndices(std::move(idx16));
                }
                else
                {
                    out->AdoptIndices(std::move(indices));
                }
            }
            else
            {
                // (b) Expanded: rewrite vertex stream in triangle order, drop indices
                const uint32_t stride = layout.GetStride();
                std::vector<std::uint8_t> expandedVB;
                expandedVB.reserve(static_cast<size_t>(expandedCount) * stride);

                auto appendByIndex = [&](std::uint32_t idx)
                {
                    const size_t srcOff = static_cast<size_t>(idx) * stride;
                    expandedVB.insert(expandedVB.end(),
                                      interleaved.begin() + srcOff,
                                      interleaved.begin() + srcOff + stride);
                };
                for (std::uint32_t idx : indices) appendByIndex(idx);

                out->AddInterleavedBindingMove(std::move(layout), std::move(expandedVB), expandedCount);
                // No indices for expanded geometry
            }
        }
        else // Separate (SoA)
        {
            // Separate: create one binding per present attribute, adopt raw float arrays as bytes.
            // POSITION (always)
            {
                // Convert float vector -> raw bytes (no extra copy: we move the underlying storage)
                std::vector<std::uint8_t> bytes(sizeof(float) * posSoA.size());
                std::memcpy(bytes.data(), posSoA.data(), bytes.size());
                out->AddSeparateAttributeMove(ShaderDataType::Float3, "a_Position",
                                             std::move(bytes), uniqueCount);
            }
            if (hasNorm)
            {
                std::vector<std::uint8_t> bytes(sizeof(float) * nrmSoA.size());
                std::memcpy(bytes.data(), nrmSoA.data(), bytes.size());
                out->AddSeparateAttributeMove(ShaderDataType::Float3, "a_Normal",
                                             std::move(bytes), uniqueCount);
            }
            if (hasTex)
            {
                std::vector<std::uint8_t> bytes(sizeof(float) * uvSoA.size());
                std::memcpy(bytes.data(), uvSoA.data(), bytes.size());
                out->AddSeparateAttributeMove(ShaderDataType::Float2, "a_TexCoord",
                                             std::move(bytes), uniqueCount);
            }

            if (keepIndexed)
            {
                if (FitsInU16(uniqueCount))
                {
                    std::vector<uint16_t> idx16; idx16.reserve(indices.size());
                    for (uint32_t v : indices) idx16.push_back(static_cast<uint16_t>(v));
                    out->AdoptIndices(std::move(idx16));
                }
                else
                {
                    out->AdoptIndices(std::move(indices));
                }
            }
            else
            {
                // Expanded SoA: rebuild attribute arrays in triangle order, no indices.
                // Recreate expanded arrays, then replace the bindings.
                std::vector<float> posExp; posExp.reserve(expandedCount*3);
                std::vector<float> nrmExp; if (hasNorm) nrmExp.reserve(expandedCount*3);
                std::vector<float> uvExp;  if (hasTex)  uvExp.reserve(expandedCount*2);

                auto copyByIndex = [&](std::uint32_t idx)
                {
                    // positions
                    posExp.insert(posExp.end(), {
                        posSoA[idx*3+0], posSoA[idx*3+1], posSoA[idx*3+2]
                    });
                    // normals
                    if (hasNorm)
                    {
                        nrmExp.insert(nrmExp.end(), {
                            nrmSoA[idx*3+0], nrmSoA[idx*3+1], nrmSoA[idx*3+2]
                        });
                    }
                    // uvs
                    if (hasTex)
                    {
                        uvExp.insert(uvExp.end(), {
                            uvSoA[idx*2+0], uvSoA[idx*2+1]
                        });
                    }
                };
                for (uint32_t idx : indices) copyByIndex(idx);

                // Replace the just-added bindings with expanded versions (simpler path: add new bindings).
                // (POSITION)
                {
                    std::vector<std::uint8_t> bytes(sizeof(float) * posExp.size());
                    std::memcpy(bytes.data(), posExp.data(), bytes.size());
                    // Simple way: create a new MeshData and swap… but to keep it direct:
                    // Add another binding and ignore the previous one (or rebuild a new MeshData).
                    // Here, for simplicity, we rebuild a fresh MeshData:
                    MeshData* expanded = new MeshData(); 
                    expanded->SetUsage(out->Usage());
                    expanded->AddSeparateAttributeMove(ShaderDataType::Float3, "a_Position",
                                                      std::move(bytes), expandedCount);
                    if (hasNorm)
                    {
                        std::vector<std::uint8_t> b(sizeof(float) * nrmExp.size());
                        std::memcpy(b.data(), nrmExp.data(), b.size());
                        expanded->AddSeparateAttributeMove(ShaderDataType::Float3, "a_Normal",
                                                          std::move(b), expandedCount);
                    }
                    if (hasTex)
                    {
                        std::vector<std::uint8_t> b(sizeof(float) * uvExp.size());
                        std::memcpy(b.data(), uvExp.data(), b.size());
                        expanded->AddSeparateAttributeMove(ShaderDataType::Float2, "a_TexCoord",
                                                          std::move(b), expandedCount);
                    }
                    // Move the freshly built expanded mesh into 'out'
                    delete out; 
                    out = expanded; 
                }
            }
        }

        return out; // MeshData is move-only; return value optimization / NRVO applies
    }
}
