#include <assert.h>
#include <core/SpellcoreRenderingBackend.h>
#include <core/components/SCGeometryData.h>
#include <core/components/SCRenderTarget.h>
#include <core/components/GPUBufferLayout.h>
#include <core/systems/SCRendererResourceManager.h>
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
        _renderingBackendFactory = SpellcoreRenderingBackend::Get(); 
    }

    SCGeometryHandle SCRendererResourceManager::CreateGeometryResource(const SCGeometryData& meshData)
    {
        auto geometryBuffer = _renderingBackendFactory->CreateGeometryBuffer();
        const auto& vertexAttribBuffers = meshData.GetMeshDataBuffers();

        for (const auto& vaBuffer : vertexAttribBuffers)
        {
            GPUBuffer* gpuBuffer = _renderingBackendFactory->CreateGPUBuffer();

            gpuBuffer->SetBufferData<uint8_t>(vaBuffer.bytes, 0);
            gpuBuffer->SetLayout(vaBuffer.layout);
            geometryBuffer->AddAttributeBuffer(gpuBuffer);
        }

        GPUBuffer* indexBuffer = _renderingBackendFactory->CreateGPUBuffer();

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
        _geometryBuffers[geometryBuffer->GetHandle()] = geometryBuffer; 
        return geometryBuffer->GetHandle();
    }


    bool SCRendererResourceManager::UpdateGeometryResource(const SCGeometryHandle& scGeoHandle, const SCGeometryData& scGeoData)
    {
        return false; 
    }

    bool SCRendererResourceManager::DestroyGeometryResource(SCGeometryHandle& scGeoHandle)
    {
        auto it = _geometryBuffers.find(scGeoHandle); 

        if (it == _geometryBuffers.end())
        {
            return false; 

        }

        _geometryBuffers.erase(scGeoHandle); 
        delete it->second;
        return true;
    }

    SCTextureHandle SCRendererResourceManager::CreateTexture(const SCTextureImageDesc& scTextureDesc)
	{
        return 0; 
	}

	SCTextureHandle SCRendererResourceManager::CreateTexture(const SCTextureImageDesc& scTextureDesc, const SCImageData* initialData)
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

    SCRenderTargetHandle SCRendererResourceManager::CreateRenderTarget(const SCRTDescription& renderTargetDesc)
    {
        IRenderTarget* renderTarget  = _renderingBackendFactory->CreateRenderTarget(); 

        return renderTarget->GetRenderTargetHandle();
    }

    bool SCRendererResourceManager::DestroyRenderTarget(SCRenderTargetHandle handle)
    {
        auto it = _renderTargets.find(handle);

        if (it != _renderTargets.end())
        {
           // _renderResourceManager->DestroyRenderTarget(handle);
            _renderTargets.erase(it);
            return true;
        }

        return false;
    }

    bool SCRendererResourceManager::UseRenderTarget(const SCRenderTargetHandle& scrtHandle)
    {

        auto it = _renderTargets.find(scrtHandle);

        if (it != _renderTargets.end())
        {
            return true;
        }

        return false;
    }

    const SCRTDescription* SCRendererResourceManager::GetRenderTargetDesc(const SCRenderTargetHandle& handle) const
    {
        auto it = _renderTargets.find(handle);
        if (it != _renderTargets.end())
        {
            return &it->second;
        }
        
        return nullptr; 
    }

    SCRendererResourceManager::~SCRendererResourceManager()
    {
      
    }
}