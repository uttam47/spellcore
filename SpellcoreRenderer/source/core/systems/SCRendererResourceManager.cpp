#include <core/components/GPUBufferLayout.h>
#include <core/SpellcoreRenderingBackend.h>
#include <core/systems/SCRendererResourceManager.h>
#include <core/components/SCGeometryData.h>
#include <core/components/SpellcoreRenderDataTypes.h>

namespace AnalyticalApproach::Spellcore
{
    SCRendererResourceManager* SCRendererResourceManager::STATIC_INSTANCE = nullptr;

    SCRendererResourceManager* SCRendererResourceManager::GetInstance()
    {
        if (STATIC_INSTANCE == nullptr)
        {
            STATIC_INSTANCE = new SCRendererResourceManager(); 
        }

        return STATIC_INSTANCE; 
    }

    void SCRendererResourceManager::DestroyInstance()
    {
        if (STATIC_INSTANCE != nullptr)
        {
            delete STATIC_INSTANCE;
        }

        STATIC_INSTANCE = nullptr; 
    }

    SCRendererResourceManager::SCRendererResourceManager()
    {
        _renderingBackend = SpellcoreRenderingBackend::Get(); 
    }

    SCGeometryHandle SCRendererResourceManager::CreateSpellcoreGeometry(const SCGeometryData& meshData)
    {
        auto geometryBuffer = _renderingBackend->CreateGeometryBuffer();
        const auto& vertexAttribBuffers = meshData.GetMeshDataBuffers();

        for (const auto& vaBuffer : vertexAttribBuffers)
        {
            GPUBuffer* gpuBuffer = _renderingBackend->CreateGPUBuffer();

            gpuBuffer->SetBufferData<byte>(vaBuffer.bytes, 0);
            gpuBuffer->SetLayout(vaBuffer.layout);
            geometryBuffer->AddAttributeBuffer(gpuBuffer);
        }

        GPUBuffer* indexBuffer = _renderingBackend->CreateGPUBuffer();

        GPUBufferElement gpuBufferElement;
        gpuBufferElement.name = "Vertex_Index";

        if (meshData.GetIndexType() == IndexType::UInt16)
        {
            gpuBufferElement.type = SCDataType::UShort;
            gpuBufferElement.size = ShaderDataTypeSize(SCDataType::UShort); // 2
            indexBuffer->SetBufferData<uint16_t>(meshData.IndicesU16());
        }
        else if (meshData.GetIndexType() == IndexType::UInt32)
        {
            gpuBufferElement.type = SCDataType::UInt;
            gpuBufferElement.size = ShaderDataTypeSize(SCDataType::UInt);   // 4
            indexBuffer->SetBufferData<uint32_t>(meshData.IndicesU32());
        }

        GPUBufferLayout bufferLayout({ gpuBufferElement });
        bufferLayout.gpuBufferSubType = GPUBufferSubType::INDEX_DATA;
        bufferLayout.gpuBufferType = GPUBufferType::VERTEX_DATA_BUFFER;
        bufferLayout.GetElements();
        indexBuffer->SetLayout(bufferLayout);

        geometryBuffer->AddIndexBuffer(indexBuffer);

        _geometryBuffers[geometryBuffer->GetId()] = geometryBuffer; 
    }


    bool SCRendererResourceManager::UpdateSpellcoreGeometry(const SCGeometryHandle& scGeoHandle, const SCGeometryData& scGeoData)
    {
        return false; 
    }

    bool SCRendererResourceManager::DestroySpellcoreGeometry(SCGeometryHandle& scGeoHandle)
    {

        return false;
    }

    SCTextureHandle SCRendererResourceManager::CreateSpellcoreTexture(const SCTextureDesc& scTextureDesc)
	{
        return 0; 
	}

	SCTextureHandle SCRendererResourceManager::CreateSpellcoreTexture(const SCTextureDesc& scTextureDesc, const SCImageData* initialData)
	{
        return 0;
	}

	SCMaterialHandle SCRendererResourceManager::CreateSpellcoreMaterial()
	{
        return 0; 
	}

    //TODO: Pull Bifurcate Render Pipeline and Shader Manager into two different entities. 
	SCShaderHandle   SCRendererResourceManager::CreateSpellcoreShader()
	{
        return 0; 
	}

}