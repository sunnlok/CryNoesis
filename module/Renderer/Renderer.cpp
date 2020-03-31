#include "StdAfx.h"
#include "Renderer.h"
#include <limits>

#include <NsCore/Ptr.h>
#include <NsGui/IView.h>
#include <NsGui/IRenderer.h>
#include <NsRender/RenderTarget.h>
#include <NsRender/Texture.h>
#include "Texture.h"

#include <dxgiformat.h>

#include "LayoutDefinitions.h"
#include <variant>
#include "../RenderDll/Common/CryNameR.h"
#include "CryCore/Containers/CryArray.h"

using namespace Cry;
using namespace Ns::LayoutDefinitions;

static std::array<Ns::SShaderInfo, Noesis::Shader::Count> g_shaderInfo;
static CCryNameTSCRC gTechCrc("Noesis");

constexpr auto gSizeTexDimCB = 4 * sizeof(float);
constexpr auto gSizeEffectCB = 8 * sizeof(float);
constexpr auto gSizePixelCB = 13 * sizeof(float);
constexpr auto gSizeVertexCB = 16 * sizeof(float);





Ns::CRenderDevice::CRenderDevice()
{
	m_pPipeline = Renderer::Pipeline::GetOrCreateCustomPipeline();
	m_pResourceProvider = m_pPipeline->GetResourceProvider();

	//m_pNoesisShader = gEnv->pRenderer->EF_LoadShader("Noesis", g_layoutInfoList[0].mask);

	m_pPipeline->ExecuteRenderThreadCommand([=]() {

		int i = 0;
		for (auto& desc : g_layoutInfoList)
		{
			CryLogAlways("Layout:");

			

			g_shaderInfo[i].effectID = desc.shader;
			std::visit([=, &i](auto& descs) {
				for (auto& descript : descs)
				{
					CryLogAlways(descript.semanticName);
				}

				g_shaderInfo[i].layout = m_pResourceProvider->RegisterLayout(descs.data(), descs.size());


			}, desc.descriptions);
			g_shaderInfo[i].mask = desc.mask;	
			g_shaderInfo[i].pShader = gEnv->pRenderer->EF_LoadShader("Noesis", 0, desc.mask);

			++i;
		}
	});
}


Ns::CRenderDevice::~CRenderDevice()
{

}

static Noesis::DeviceCaps g_Capabilities{
	0.5f
};

const Noesis::DeviceCaps& Ns::CRenderDevice::GetCaps() const
{
	return g_Capabilities;
}

Noesis::Ptr<Noesis::RenderTarget> Ns::CRenderDevice::CreateRenderTarget(const char* label, uint32_t width, uint32_t height, uint32_t sampleCount)
{
	Renderer::SRTCreationParams params = {
		label,
		width,
		height,
		Col_Transparent,
		eTT_2D,
		FT_NOMIPS | FT_DONT_STREAM | FT_USAGE_RENDERTARGET,
		ETEX_Format::eTF_R8G8B8A8
	};

	auto pTexture = m_pResourceProvider->CreateRenderTarget(params);
	if (!pTexture)
		return nullptr;

	return Noesis::MakePtr<Ns::CRenderTarget>(std::move(pTexture));
}

Noesis::Ptr<Noesis::RenderTarget> Ns::CRenderDevice::CloneRenderTarget(const char* label, Noesis::RenderTarget* surface)
{
	auto pOriginalTarget = surface->GetTexture();
	auto pOriginalTexWrapper = static_cast<Ns::CTextureWrapper*>(pOriginalTarget);
	auto pOriginalTex = pOriginalTexWrapper->GetActualTexture();

	auto width = pOriginalTex->GetWidth();
	auto height = pOriginalTex->GetHeight();

	Renderer::SRTCreationParams params = {
		label,
		pOriginalTex->GetWidth(),
		pOriginalTex->GetHeight(),
		Col_Transparent,
		eTT_2D,
		FT_NOMIPS | FT_DONT_STREAM | FT_USAGE_RENDERTARGET,
		ETEX_Format::eTF_R8G8B8A8
	};

	auto pTexture = m_pResourceProvider->CreateRenderTarget(params);
	if (!pTexture)
		return nullptr;

	RectI dimensions{width,height, 0, 1};
	auto col = Col_Transparent;
	gEnv->pRenderer->CopyTextureRegion(pOriginalTex, dimensions, pTexture, dimensions, col, 0);

	return Noesis::MakePtr<Ns::CRenderTarget>(std::move(pTexture));
}

Noesis::Ptr<Noesis::Texture> Ns::CRenderDevice::CreateTexture(const char* label, uint32_t width, uint32_t height, uint32_t numLevels, Noesis::TextureFormat::Enum format, const void** data)
{

	ETEX_Format texFormat = format == Noesis::TextureFormat::RGBA8 ? ETEX_Format::eTF_R8G8B8A8 : ETEX_Format::eTF_R8;
	auto pTexture = gEnv->pRenderer->CreateTexture(label, width, height, numLevels, data ? (uint8*)data[0] : nullptr, texFormat, 0);

	if (!pTexture)
		return nullptr;

	_smart_ptr<ITexture> pSmartText;
	pSmartText.Assign_NoAddRef(pTexture);

	return Noesis::MakePtr<Ns::CTextureWrapper>(std::move(pSmartText));
}

void Ns::CRenderDevice::UpdateTexture(Noesis::Texture* texture, uint32_t level, uint32_t x, uint32_t y, uint32_t width, uint32_t height, const void* data)
{
	auto pTexWrapper = static_cast<Ns::CTextureWrapper*>(texture);
	auto pTex = pTexWrapper->GetActualTexture();

	gEnv->pRenderer->UpdateTextureInVideoMemory(pTex->GetTextureID(), (unsigned char*)data, x, y, width, height, pTex->GetTextureSrcFormat());
}

void Ns::CRenderDevice::BeginRender(bool offscreen)
{
	m_bRenderingOffscreen = offscreen;

	m_pCurrentView->idxBufferOffset = 0;
	m_pCurrentView->vtxBufferOffset = 0;
	if (offscreen)
		BeginOffscreenRender();
	else
		BeginActualRender();

}

void Ns::CRenderDevice::SetRenderTarget(Noesis::RenderTarget* surface)
{
	m_pCurrentRenderTarget = surface;
}

void Ns::CRenderDevice::BeginTile(const Noesis::Tile& tile, uint32_t surfaceWidth, uint32_t surfaceHeight)
{
	
}

void Ns::CRenderDevice::EndTile()
{
	//The method or operation is not implemented.
}

void Ns::CRenderDevice::ResolveRenderTarget(Noesis::RenderTarget* surface, const Noesis::Tile* tiles, uint32_t numTiles)
{
	//The method or operation is not implemented.
}

void Ns::CRenderDevice::EndRender()
{
	if (m_bRenderingOffscreen)
		EndOffscreenRender();
	else
		EndActualRender();
}

void* Ns::CRenderDevice::MapVertices(uint32_t bytes)
{

	uint8* pStream = (uint8 *)m_pResourceProvider->BufferBeginWrite(m_pCurrentView->activeVertexBuffer);
	pStream = pStream + m_pCurrentView->vtxBufferOffset;

	m_pCurrentView->vtxBufferDrawPos = m_pCurrentView->vtxBufferOffset;
	m_pCurrentView->vtxBufferOffset += bytes;

	return pStream;
}

void Ns::CRenderDevice::UnmapVertices()
{
	m_pResourceProvider->BufferEndWrite(m_pCurrentView->activeVertexBuffer);
}

void* Ns::CRenderDevice::MapIndices(uint32_t bytes)
{
	uint8* pStream = (uint8*)m_pResourceProvider->BufferBeginWrite(m_pCurrentView->activeIndexBuffer);
	pStream = pStream + m_pCurrentView->idxBufferOffset;

	m_pCurrentView->idxBufferDrawPos = m_pCurrentView->idxBufferOffset;
	m_pCurrentView->idxBufferOffset += bytes;

	return pStream;
}

void Ns::CRenderDevice::UnmapIndices()
{
	m_pResourceProvider->BufferEndWrite(m_pCurrentView->activeIndexBuffer);
}


void Ns::CRenderDevice::DrawBatch(const Noesis::Batch& batch)
{
	using namespace Renderer::Pipeline::Pass;

	//The method or operation is not implemented.
	auto& info = g_shaderInfo[batch.shader.v];

	SDrawParamsExternalBuffers bufferParams;
	bufferParams.inputBuffer = m_pCurrentView->activeVertexBuffer;
	bufferParams.idxBuffer = m_pCurrentView->activeIndexBuffer;

	bufferParams.inputLayout = info.layout;
	bufferParams.inputSize		= batch.numVertices;
	bufferParams.inputOffset = 0;//batch.vertexOffset + m_pCurrentView->vtxBufferDrawPos;
	bufferParams.inputStride = g_layoutInfoList[batch.shader.v].stride;

	bufferParams.idxSize = batch.numIndices;
	bufferParams.idxOffset = batch.startIndex + m_pCurrentView->idxBufferDrawPos / 2;
	bufferParams.isIDX32 = false;

	StaticDynArray <SMultiVlaueConstantBuffer, 4> buffers;

	StaticDynArray<SBufferValuePtr, 1> matrixBuffers;
	//if (m_pCurrentView->vertexCBHash != batch.projMtxHash)
	{
		SBufferValuePtr matrixPtr{
		(uint8*)batch.projMtx,
		16 * sizeof(float)
		};
		matrixBuffers.push_back(matrixPtr);

		SMultiVlaueConstantBuffer matrixBuffer{
			m_pCurrentView->vertexCB,
			true,
			(Renderer::Shader::EConstantSlot)0,
			Renderer::Shader::EShaderStages::ssVertex,
			TArray(&matrixBuffers[0], matrixBuffers.size())
		};
		
		buffers.push_back(matrixBuffer);
		m_pCurrentView->vertexCBHash = batch.projMtxHash;
	}
	
	StaticDynArray<SBufferValuePtr, 3> pixelBuffers;
	//if (batch.rgba != 0 || batch.radialGrad != 0 || batch.opacity != 0)
	{
		uint32 hash = batch.rgbaHash ^ batch.radialGradHash ^ batch.opacityHash;
		if (m_pCurrentView->pixelCBHash != hash)
		{
			if (batch.rgba != 0)
			{
				SBufferValuePtr pixelBuffer{
				(uint8*)batch.rgba,
					4 * sizeof(float)
				};
				pixelBuffers.push_back(pixelBuffer);
			}

			if (batch.radialGrad != 0)
			{
				SBufferValuePtr pixelBuffer{
				(uint8*)batch.radialGrad,
					8 * sizeof(float)
				};
				pixelBuffers.push_back(pixelBuffer);
			}

			if (batch.opacity != 0)
			{
				SBufferValuePtr pixelBuffer{
				(uint8*)batch.opacity,
					sizeof(float)
				};
				pixelBuffers.push_back(pixelBuffer);
			}

			SMultiVlaueConstantBuffer pixelBuffer{
			m_pCurrentView->pixelCB,
			true,
			(Renderer::Shader::EConstantSlot)0,
			Renderer::Shader::EShaderStages::ssPixel,
			TArray(&pixelBuffers[0], pixelBuffers.size())
			};

			buffers.push_back(pixelBuffer);

			m_pCurrentView->pixelCBHash = hash;
		}

	}

	StaticDynArray<SBufferValuePtr, 1> textureBuffers;
	float textureData[4];
	if (batch.glyphs != 0 || batch.image != 0)
	{
		auto pTexture = static_cast<Ns::CTextureWrapper*>(batch.glyphs ? batch.glyphs : batch.image);
		uint32 width = pTexture->GetWidth();
		uint32 height = pTexture->GetHeight();
		uint32 hash = (width << 16) | height;

		//if (m_pCurrentView->texDimensionsCBHash != hash)
		{
			textureData[0] = (float)width;
			textureData[1] = (float)height;
			textureData[2] = 1.0f / width;
			textureData[3] = 1.0f / height;

			SBufferValuePtr textureBuffer{
				(uint8*)textureData,
					4 * sizeof(float)
			};
			textureBuffers.push_back(textureBuffer);

			SMultiVlaueConstantBuffer textureValues{
			m_pCurrentView->textDimCB,
			true,
			(Renderer::Shader::EConstantSlot)1,
			Renderer::Shader::EShaderStages::ssVertex | Renderer::Shader::EShaderStages::ssPixel,
			TArray(&textureBuffers[0], textureBuffers.size())
			};

			buffers.push_back(textureValues);
			m_pCurrentView->texDimensionsCBHash = hash;
		}

	}

	StaticDynArray<SBufferValuePtr, 1> effectBuffers;
	if (batch.effectParamsSize != 0)
	{
		//if (m_pCurrentView->effectCBHash != batch.effectParamsHash)
		{
			SBufferValuePtr effectBuffer{
				(uint8*)batch.effectParams,
					batch.effectParamsSize * sizeof(float)
			};

			effectBuffers.push_back(effectBuffer);

			SMultiVlaueConstantBuffer effectValues{
			m_pCurrentView->effectCB,
			true,
			(Renderer::Shader::EConstantSlot)2,
			Renderer::Shader::EShaderStages::ssPixel,
			TArray(&effectBuffers[0], effectBuffers.size())
			};

			buffers.push_back(effectValues);

			m_pCurrentView->effectCBHash = batch.effectParamsHash;
		}
	}

	SInlineMultiValueConstantParams constantParams;
	constantParams.buffers = TArray(&buffers[0], buffers.size());

	SShaderParams shaderParams;
	shaderParams.pShader = info.pShader;//m_pNoesisShader;
	shaderParams.techniqueLCCRC = gTechCrc.get();
	shaderParams.rtMask = info.mask;


	StaticDynArray<STextureParam, 5> texParams;
	StaticDynArray<SSamplerParam, 5> samplerParams;
	

	if (batch.pattern != 0)
	{
		STextureParam param{
			static_cast<Ns::CTextureWrapper*>(batch.pattern)->GetActualTexture(),
			0
		};
		texParams.push_back(texParams);
		SSamplerParam samplerParam{
			0,
			0
		};
		samplerParams.emplace_back(samplerParam); //TODO: Add sampler registration to stage api
	}

	if (batch.ramps != 0)
	{
		STextureParam param{
			static_cast<Ns::CTextureWrapper*>(batch.ramps)->GetActualTexture(),
			1
		};
		texParams.push_back(texParams);
		SSamplerParam samplerParam{
			0,
			1
		};
		samplerParams.emplace_back(samplerParam); //TODO: Add sampler registration to stage api
	}

	if (batch.image != 0)
	{
		STextureParam param{
			static_cast<Ns::CTextureWrapper*>(batch.image)->GetActualTexture(),
			2
		};
		texParams.push_back(texParams);
		SSamplerParam samplerParam{
			0,
			2
		};
		samplerParams.emplace_back(samplerParam); //TODO: Add sampler registration to stage api
	}
	if (batch.glyphs != 0)
	{
		STextureParam param{
			static_cast<Ns::CTextureWrapper*>(batch.glyphs)->GetActualTexture(),
			3
		};
		texParams.push_back(texParams);
		SSamplerParam samplerParam{
			0,
			3
		};
		samplerParams.emplace_back(samplerParam); //TODO: Add sampler registration to stage api
	}
	if (batch.shadow != 0)
	{
		STextureParam param{
			static_cast<Ns::CTextureWrapper*>(batch.shadow)->GetActualTexture(),
			4
		};
		texParams.push_back(texParams);
		SSamplerParam samplerParam{
			0,
			4
		};
		samplerParams.emplace_back(samplerParam); //TODO: Add sampler registration to stage api
	}

	shaderParams.textures = TArray(&texParams[0], texParams.size());
	shaderParams.samplerStates = TArray(&samplerParams[0], samplerParams.size());
	/*string maskDBG;
	for (uint64 i =0; i < (uint64)Gauss::Count; ++i)
	{
		uint64 chckMsk = 1ull << i;
		if (info.mask & chckMsk)
		{
			maskDBG.append(gMaskNames[i]);
			maskDBG.append(" ");
		}

	}
	CryLogAlways("Shader mask: %s ", maskDBG);*/

	SPrimitiveParams primParams(shaderParams, constantParams, bufferParams);

	m_pPipeline->RT_AddPrimitive(*m_pCurrentView->stage, primParams);
}

void Cry::Ns::CRenderDevice::BeginActualRender()
{
	m_pPipeline->RT_BeginPass(*m_pCurrentView->stage, m_pCurrentView->viewPass);
}

void Cry::Ns::CRenderDevice::EndActualRender()
{
	m_pPipeline->RT_EndPass(*m_pCurrentView->stage, true);
}

void Cry::Ns::CRenderDevice::BeginOffscreenRender()
{

}

void Cry::Ns::CRenderDevice::EndOffscreenRender()
{

}

void Cry::Ns::CRenderDevice::AddView(SViewInitParams viewParams)
{
	m_pPipeline->ExecuteRenderThreadCommand([this, viewParams]() { RT_AddView(viewParams); });
}

void Cry::Ns::CRenderDevice::RT_AddView(SViewInitParams viewParams)
{
	auto pViewData = std::make_unique<SPerViewRenderData>();
	pViewData->view = viewParams.view;

	Renderer::Pipeline::SStageCallbacks callbacks{
		[pView = pViewData.get(), this] (Renderer::Pipeline::StageRenderArguments& args){ RT_RenderView(pView, args); },
		nullptr,
		[pView = pViewData.get(), this](Renderer::Pipeline::StageDestructionsArguments& args){ RT_DestroyView(pView, args); }
	};

	string name;
	name.Format("Noesis_View%u", m_viewID++);

	auto pStage = m_pPipeline->RT_CreateRenderStage(name, CCrc32::Compute(name.c_str(),name.size(), 0), std::move(callbacks));
	if (!pStage)
	{
		CryLogAlways("Failed to create noesis stage: %s", name);
		return;
	}

	name.append("_MainPass");

	Renderer::Pipeline::Pass::SPassCreationParams passParams;
	passParams.passName = name;
	passParams.passCrc = CCrc32::Compute(name);

	string rtName = name;
	rtName.append("_Color");

	//Create render targets
	Renderer::SRTCreationParams params = {
		rtName,
		viewParams.width,
		viewParams.height,
		Col_Transparent,
		eTT_2D,
		FT_NOMIPS | FT_DONT_STREAM | FT_USAGE_RENDERTARGET,
		ETEX_Format::eTF_R8G8B8A8
	};

	pViewData->viewColorTarget = m_pResourceProvider->CreateRenderTarget(params);

	name.append("_Depth");

	params.rtName = name;
	params.flags = FT_NOMIPS | FT_DONT_STREAM | FT_USAGE_DEPTHSTENCIL;
	params.format = ETEX_Format::eTF_A8;

	pViewData->viewDepthTarget = m_pResourceProvider->CreateRenderTarget(params);

	passParams.pColorTarget = pViewData->viewColorTarget;
	passParams.pDepthsTarget = pViewData->viewDepthTarget;
	passParams.viewPort = SRenderViewport(0, 0, viewParams.width, viewParams.height);

	pViewData->viewPass = m_pPipeline->RT_AllocatePass(*pStage, passParams);


	Renderer::Buffers::SBufferParams idxBufferParams(DYNAMIC_IB_SIZE, 1, Renderer::Buffers::EBufferBindType::Index);
	pViewData->activeIndexBuffer = m_pResourceProvider->CreateOrUpdateBuffer(idxBufferParams, pViewData->activeIndexBuffer);

	Renderer::Buffers::SBufferParams vtxBufferParams(DYNAMIC_VB_SIZE, 1, Renderer::Buffers::EBufferBindType::Vertex);
	pViewData->activeVertexBuffer = m_pResourceProvider->CreateOrUpdateBuffer(vtxBufferParams, pViewData->activeVertexBuffer);


	//Create constant buffers
	string vtxCBDbg = name + "_VTX_CB";
	pViewData->vertexCB = m_pResourceProvider->CreateConstantBuffer(gSizeVertexCB, vtxCBDbg);

	string effectCBDbg = name + "_EFFECT_CB";
	pViewData->effectCB = m_pResourceProvider->CreateConstantBuffer(gSizeEffectCB, effectCBDbg);
	string pixelCBDbg = name + "_PIXEL_CB";
	pViewData->pixelCB	= m_pResourceProvider->CreateConstantBuffer(gSizePixelCB, pixelCBDbg);
	string texDimCBDbg = name + "_TEX_CB";
	pViewData->textDimCB = m_pResourceProvider->CreateConstantBuffer(gSizeTexDimCB, texDimCBDbg);

	pViewData->stage = pStage;
	pViewData->view->GetRenderer()->Init(this);
	m_perViewRenderData.push_back(std::move(pViewData));
	
}

void Cry::Ns::CRenderDevice::RT_RenderView(SPerViewRenderData* pViewData, Renderer::Pipeline::StageRenderArguments& args)
{
	auto pRenderer = pViewData->view->GetRenderer();
	if (pRenderer->UpdateRenderTree())
	{
		m_pCurrentView = pViewData;
		pRenderer->RenderOffscreen();
		pRenderer->Render();


		m_pCurrentView = nullptr;
	}
}

void Cry::Ns::CRenderDevice::RT_DestroyView(SPerViewRenderData* pViewData, Renderer::Pipeline::StageDestructionsArguments& args)
{
	for (auto iter = m_perViewRenderData.begin(); iter != m_perViewRenderData.end(); ++iter)
	{
		if (iter->get() != pViewData)
			continue;

		m_pResourceProvider->FreeBuffer(pViewData->activeIndexBuffer);
		m_pResourceProvider->FreeBuffer(pViewData->activeVertexBuffer);

		m_pResourceProvider->FreeConstantBuffer(pViewData->vertexCB);
		m_pResourceProvider->FreeConstantBuffer(pViewData->effectCB);
		m_pResourceProvider->FreeConstantBuffer(pViewData->pixelCB);
		m_pResourceProvider->FreeConstantBuffer(pViewData->textDimCB);

		pViewData->view->GetRenderer()->Shutdown();

		m_perViewRenderData.erase(iter);	

		//Do a potential surface clear here 
	}
}

Cry::Renderer::Pipeline::Pass::SInlineConstantParams Cry::Ns::CRenderDevice::CreateConstantParameters(const Noesis::Batch& batch)
{
	///*Renderer::Pipeline::Pass::SInlineMultiValueConstantParams params;

	//if (m_pCurrentView->vertexCBHash != batch.projMtxHash)
	//{
	//	SMultiVlaueConstantBuffer buffer;
	//	buffer.externalBuffer = (uint8*)batch.projMtx;

	//}*/


	//m_pResourceProvider->CreateOrUpdateBuffer()
	return SInlineConstantParams();

}

