#include "StdAfx.h"
#include "Renderer.h"
#include <limits>

#include <NsCore/Ptr.h>
#include <NsGui/IView.h>
#include <NsGui/IRenderer.h>
#include <NsRender/RenderTarget.h>
#include <NsRender/Texture.h>
#include "Texture.h"

#include "LayoutDefinitions.h"
#include <variant>
#include "../RenderDll/Common/CryNameR.h"
#include "CryCore/Containers/CryArray.h"
#include "NsMath/Matrix.h"


#include "StateDefinitions.h"

using namespace Cry;
using namespace Ns::LayoutDefinitions;
using namespace Cry::Noesis::Renderer;

static std::array<Ns::SShaderInfo, ::Noesis::Shader::Count> g_shaderInfo;
static CCryNameTSCRC gTechCrc("Noesis");

constexpr auto gSizeTexDimCB = 4 * sizeof(float);
constexpr auto gSizeEffectCB = 16 * sizeof(float);
constexpr auto gSizePixelCB = 12 * sizeof(float);
constexpr auto gSizeVertexCB = 16 * sizeof(float);

static SamplerStateHandle gSamplerStateHandles[64];


static void RegisterSamplers(Renderer::IStageResourceProvider* pResourceProvider)
{
	int i = 0;
	for (uint8_t minmag = 0; minmag < ::Noesis::MinMagFilter::Count; minmag++)
	{
		for (uint8_t mip = 0; mip < ::Noesis::MipFilter::Count; mip++)
		{
			auto filter = gSamplerFilter[i++];

			for (uint8_t uv = 0; uv < WrapMode::Count; uv++)
			{
				auto addresses = gSamplerAdresses[uv];

				Renderer::Sampler::SState state{
				filter.min,
				filter.mag,
				filter.mip,
				addresses.u,
				addresses.v,
				Renderer::Sampler::EAddressMode::Clamp,
				state.bNeverKeep = true
				};
				
				::Noesis::SamplerState s = { {uv, minmag, mip} };
				gSamplerStateHandles[s.v] = pResourceProvider->RegisterSamplers(state);
			}
		}
	}
}

static void RegisterLayouts(Renderer::IStageResourceProvider* pResourceProvider)
{
	
	for (int i = 0; i < g_layoutInfoList.size(); ++i )
	{
		auto& layoutInfo = g_layoutInfoList[i];
		auto& shaderInfo = g_shaderInfo[i];
		shaderInfo.effectID = layoutInfo.shader;
		
		std::visit([&shaderInfo, pResourceProvider](auto& descs) {
			shaderInfo.layout = pResourceProvider->RegisterLayout(descs.data(), descs.size());
		}, layoutInfo.descriptions);
		shaderInfo.mask = layoutInfo.mask;

		shaderInfo.pShader = gEnv->pRenderer->EF_LoadShader("Noesis", 0, layoutInfo.mask);
	}
}


Ns::CRenderDevice::CRenderDevice()
{
	m_pPipeline = Renderer::Pipeline::GetOrCreateCustomPipeline();
	m_pResourceProvider = m_pPipeline->GetResourceProvider();

	//m_pNoesisShader = gEnv->pRenderer->EF_LoadShader("Noesis", g_layoutInfoList[0].mask);

	m_pPipeline->ExecuteRenderThreadCommand([=]() {
		RegisterLayouts(m_pResourceProvider);
		RegisterSamplers(m_pResourceProvider);
	});
}


Ns::CRenderDevice::~CRenderDevice()
{

}

static ::Noesis::DeviceCaps g_Capabilities{
	0.5f
};

const ::Noesis::DeviceCaps& Ns::CRenderDevice::GetCaps() const
{
	return g_Capabilities;
}

::Noesis::Ptr<::Noesis::RenderTarget> Ns::CRenderDevice::CreateRenderTarget(const char* label, uint32_t width, uint32_t height, uint32_t sampleCount)
{
	string textName(label);
	
	Renderer::SRTCreationParams params = {
		textName,
		width,
		height,
		Col_Transparent,
		eTT_2D,
		FT_NOMIPS | FT_DONT_STREAM | FT_USAGE_RENDERTARGET,
		ETEX_Format::eTF_R8G8B8A8,
		-1
	};

	auto pTexture = m_pResourceProvider->CreateRenderTarget(params);
	if (!pTexture)
		return nullptr;

	textName.append("_Depth");

	Renderer::SRTCreationParams depthParams = {
		textName,
		width,
		height,
		Clr_FarPlane,
		eTT_2D,
		FT_NOMIPS | FT_DONT_STREAM | FT_USAGE_DEPTHSTENCIL,
		ETEX_Format::eTF_S8,
		-1
	};

	auto pDepth = m_pResourceProvider->CreateRenderTarget(depthParams);

	return ::Noesis::MakePtr<Ns::CRenderTarget>(std::move(pTexture), std::move(pDepth));
}

::Noesis::Ptr<::Noesis::RenderTarget> Ns::CRenderDevice::CloneRenderTarget(const char* label, ::Noesis::RenderTarget* surface)
{
	auto pTarget = static_cast<Ns::CRenderTarget*>(surface);

	Renderer::SRTCreationParams params = {
		label,
		pTarget->GetColor()->GetWidth(),
		pTarget->GetColor()->GetHeight(),
		Col_Transparent,
		eTT_2D,
		FT_NOMIPS | FT_DONT_STREAM | FT_USAGE_RENDERTARGET,
		ETEX_Format::eTF_R8G8B8A8,
		-1
	};

	auto pTexture = m_pResourceProvider->CreateRenderTarget(params);
	if (!pTexture)
		return nullptr;


	return ::Noesis::MakePtr<Ns::CRenderTarget>(pTexture, _smart_ptr(pTarget->GetDepth()));
}

::Noesis::Ptr<::Noesis::Texture> Ns::CRenderDevice::CreateTexture(const char* label, uint32_t width, uint32_t height, uint32_t numLevels, ::Noesis::TextureFormat::Enum format, const void** data)
{
	ETEX_Format texFormat = format == ::Noesis::TextureFormat::RGBA8 ? ETEX_Format::eTF_R8G8B8A8 : ETEX_Format::eTF_R8;
	uint32 textureFlags = FT_NOMIPS | FT_DONT_STREAM;
	auto pTexture = gEnv->pRenderer->CreateTexture(label, width, height, 1, data ? (uint8*)((data[0])) : nullptr, texFormat, textureFlags);

	if (!pTexture)
		return nullptr;

	_smart_ptr<ITexture> pSmartText;
	pSmartText.Assign_NoAddRef(pTexture);

	if (m_pCurrentView)
		m_pCurrentView->m_pTextures.push_back(pTexture);

	return ::Noesis::MakePtr<Ns::CTextureWrapper>(std::move(pSmartText));
}

void Ns::CRenderDevice::UpdateTexture(::Noesis::Texture* texture, uint32_t level, uint32_t x, uint32_t y, uint32_t width, uint32_t height, const void* data)
{
	if (level > 0)
		return;

	auto pTexWrapper = static_cast<Ns::CTextureWrapper*>(texture);
	auto pTex = pTexWrapper->GetActualTexture();

	
	auto texFormat = pTex->GetTextureSrcFormat();
	//auto rowPitch = texFormat == ETEX_Format::eTF_R8 ? width : width * 4;

	const ::IRenderer::SUpdateRect updateRect{
		x,y,x,y,width,height
	};
	
	//gEnv->pRenderer->SF_UpdateTexture(pTex->GetTextureID(), level, 1, &updateRect, (uint8*)data, rowPitch, 0, texFormat);

	gEnv->pRenderer->UpdateTextureInVideoMemory(pTex->GetTextureID(), (unsigned char*)data, x, y, width, height, texFormat);
}

void Ns::CRenderDevice::BeginRender(bool offscreen)
{
	m_bRenderingOffscreen = offscreen;

	if (offscreen)
		BeginOffscreenRender();
	else
		BeginActualRender();

}

void Ns::CRenderDevice::SetRenderTarget(::Noesis::RenderTarget* surface)
{
	m_pCurrentRenderTarget = static_cast<Ns::CRenderTarget*>(surface);
}

void Ns::CRenderDevice::BeginTile(const ::Noesis::Tile& tile, uint32_t surfaceWidth, uint32_t surfaceHeight)
{
	//Todo: refine all of this, far to many drawvalls and different rectangle definitions ._.


	uint32_t x = tile.x;
	uint32_t y = (uint32_t)surfaceHeight - (tile.y + tile.height);

	SRenderViewport viewport{
		(int)0,
		(int)0,
		(int)surfaceWidth,
		(int)surfaceHeight,
	};
	

	Vec4_tpl<ulong>  clearRect{
		(ulong)x,
		(ulong)y,
		(ulong)x + tile.width,
		(ulong)y + tile.height,
	};

	m_pPipeline->RT_ClearSurfaceRegion(*m_pCurrentView->stage, m_pCurrentRenderTarget->GetColor(), Col_Transparent, 1, &clearRect);
	m_pPipeline->RT_ClearDepthSurfaceRegion(*m_pCurrentView->stage, m_pCurrentRenderTarget->GetDepth(), 0x00000002l, Clr_Unused.r, Val_Stencil, 1, &clearRect);

	

	Vec4_tpl<ulong>  rect{
		(ulong)x,
		(ulong)y,
		(ulong)tile.width,
		(ulong)tile.height,
	};

	

	Renderer::Pipeline::Pass::SPassParams params{
		viewport,
		m_pCurrentRenderTarget->GetColor(),
		m_pCurrentRenderTarget->GetDepth(),
		0,
		rect
	};

	m_pPipeline->RT_BeginNewPass(*m_pCurrentView->stage, params);
}

void Ns::CRenderDevice::EndTile()
{
	m_pPipeline->RT_EndPass(*m_pCurrentView->stage, true);
}

void Ns::CRenderDevice::ResolveRenderTarget(::Noesis::RenderTarget* surface, const ::Noesis::Tile* tiles, uint32_t numTiles)
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
	if (m_pCurrentView->activeVertexBuffer != Renderer::Buffers::CINVALID_BUFFER)
	{
		m_pResourceProvider->FreeBuffer(m_pCurrentView->activeVertexBuffer);
	}

	Renderer::Buffers::SBufferParams params(
		bytes,
		1,
		Renderer::Buffers::EBufferBindType::Vertex,
		Renderer::Buffers::EBufferUsage::Transient
	);

	m_pCurrentView->activeVertexBuffer = m_pResourceProvider->CreateOrUpdateBuffer(params);

	uint8* pStream = (uint8 *)m_pResourceProvider->BufferBeginWrite(m_pCurrentView->activeVertexBuffer);
	return pStream;
}

void Ns::CRenderDevice::UnmapVertices()
{
	m_pResourceProvider->BufferEndWrite(m_pCurrentView->activeVertexBuffer);
}

void* Ns::CRenderDevice::MapIndices(uint32_t bytes)
{
	if (m_pCurrentView->activeIndexBuffer != Renderer::Buffers::CINVALID_BUFFER)
	{
		m_pResourceProvider->FreeBuffer(m_pCurrentView->activeIndexBuffer);
	}

	Renderer::Buffers::SBufferParams params(
		bytes / 2,
		2,
		Renderer::Buffers::EBufferBindType::Index,
		Renderer::Buffers::EBufferUsage::Transient
	);

	m_pCurrentView->activeIndexBuffer = m_pResourceProvider->CreateOrUpdateBuffer(params);

	uint8* pStream = (uint8*)m_pResourceProvider->BufferBeginWrite(m_pCurrentView->activeIndexBuffer);
	return pStream;
}

void Ns::CRenderDevice::UnmapIndices()
{
	m_pResourceProvider->BufferEndWrite(m_pCurrentView->activeIndexBuffer);
}


void Ns::CRenderDevice::DrawBatch(const ::Noesis::Batch& batch)
{
	using namespace Renderer::Pipeline::Pass;

	//The method or operation is not implemented.
	auto& info = g_shaderInfo[batch.shader.v];
	auto& layoutInfo = g_layoutInfoList[batch.shader.v];

	SDrawParamsExternalBuffers bufferParams;
	bufferParams.inputBuffer = m_pCurrentView->activeVertexBuffer;
	bufferParams.idxBuffer = m_pCurrentView->activeIndexBuffer;

	bufferParams.inputLayout = info.layout;
	bufferParams.inputStride = layoutInfo.stride;

	bufferParams.inputSize	= batch.numVertices;
	
	bufferParams.inputOffset = 0;
	bufferParams.inputByteOffset = batch.vertexOffset;
	bufferParams.idxSize = batch.numIndices;
	bufferParams.idxOffset = batch.startIndex;
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
			Renderer::Buffers::CINVALID_BUFFER,
			true,
			(Renderer::Shader::EConstantSlot)0,
			Renderer::Shader::EShaderStages::ssVertex,
			matrixBuffers.empty() ? TArray<SBufferValuePtr>() : TArray(&matrixBuffers[0], matrixBuffers.size())
		};
		
		buffers.push_back(matrixBuffer);
		m_pCurrentView->vertexCBHash = batch.projMtxHash;
	}
	
	StaticDynArray<SBufferValuePtr, 3> pixelBuffers;
	if (batch.rgba != 0 || batch.radialGrad != 0 || batch.opacity != 0)
	{
		uint32 hash = batch.rgbaHash ^ batch.radialGradHash ^ batch.opacityHash;
		//if (m_pCurrentView->pixelCBHash != hash)
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
			Renderer::Buffers::CINVALID_BUFFER,
			true,
			(Renderer::Shader::EConstantSlot)0,
			Renderer::Shader::EShaderStages::ssPixel,
			pixelBuffers.empty() ? TArray<SBufferValuePtr>() : TArray(&pixelBuffers[0], pixelBuffers.size())
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
			Renderer::Buffers::CINVALID_BUFFER,
			true,
			(Renderer::Shader::EConstantSlot)1,
			Renderer::Shader::EShaderStages::ssVertex | Renderer::Shader::EShaderStages::ssPixel,
			textureBuffers.empty() ? TArray<SBufferValuePtr>() : TArray(&textureBuffers[0], textureBuffers.size())
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
			Renderer::Buffers::CINVALID_BUFFER,
			true,
			(Renderer::Shader::EConstantSlot)2,
			Renderer::Shader::EShaderStages::ssPixel,
			effectBuffers.empty() ? TArray<SBufferValuePtr>() : TArray(&effectBuffers[0], effectBuffers.size())
			};

			buffers.push_back(effectValues);

			m_pCurrentView->effectCBHash = batch.effectParamsHash;
		}
	}

	SInlineMultiValueConstantParams constantParams;
	constantParams.buffers = TArray(&buffers[0], buffers.size());

	SShaderParams shaderParams;
	shaderParams.pShader = info.pShader;//m_p::NoesisShader;
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
		texParams.push_back(param);
		SSamplerParam samplerParam{
			gSamplerStateHandles[batch.patternSampler.v],
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
		texParams.push_back(param);
		SSamplerParam samplerParam{
			gSamplerStateHandles[batch.rampsSampler.v],
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
		texParams.push_back(param);
		SSamplerParam samplerParam{
			gSamplerStateHandles[batch.imageSampler.v],
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
		texParams.push_back(param);
		SSamplerParam samplerParam{
			gSamplerStateHandles[batch.glyphsSampler.v],
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
		texParams.push_back(param);
		SSamplerParam samplerParam{
			gSamplerStateHandles[batch.shadowSampler.v],
			4
		};
		samplerParams.emplace_back(samplerParam); //TODO: Add sampler registration to stage api
	}

	if (!texParams.empty())
		shaderParams.textures = TArray(&texParams[0], texParams.size());
	if (!samplerParams.empty())
		shaderParams.samplerStates = TArray(&samplerParams[0], samplerParams.size());

	SPrimitiveParams primParams(shaderParams, constantParams, bufferParams);

	uint8 selectedBlendMode = batch.renderState.f.colorEnable ? batch.renderState.f.blendMode : 3;

	primParams.renderStateFlags = gDepthStateMasks[batch.renderState.f.stencilMode] | gBlendStateMask[selectedBlendMode];
	primParams.stencilState = (int)gDepthStencilMasks[batch.renderState.f.stencilMode];
	primParams.stencilRef = batch.stencilRef;

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
		ETEX_Format::eTF_R8G8B8A8,
		-1
	};

	pViewData->viewColorTarget = m_pResourceProvider->CreateRenderTarget(params);
	//pViewData->viewColorTarget = m_pResourceProvider->GetCurrentColorTarget();

	string deptRTName = name;

	deptRTName.append("_Depth");
	Renderer::SRTCreationParams depthParams = {
		deptRTName,
		viewParams.width,
		viewParams.height,
		Clr_FarPlane,
		eTT_2D,
		FT_NOMIPS | FT_DONT_STREAM | FT_USAGE_DEPTHSTENCIL,
		ETEX_Format::eTF_S8,
		-1
	};

	pViewData->viewDepthTarget = m_pResourceProvider->CreateRenderTarget(depthParams);

	passParams.pColorTarget = pViewData->viewColorTarget;
	passParams.pDepthsTarget = pViewData->viewDepthTarget;
	passParams.viewPort = SRenderViewport(0, 0, viewParams.width, viewParams.height);
	passParams.clearMask = BIT(2) | BIT(1);

	pViewData->viewWidth = viewParams.width;
	pViewData->viewHeight = viewParams.height;
	pViewData->viewPass = m_pPipeline->RT_AllocatePass(*pStage, passParams);


	/*Renderer::Buffers::SBufferParams idxBufferParams(DYNAMIC_IB_SIZE, 1, Renderer::Buffers::EBufferBindType::Index);
	pViewData->activeIndexBuffer = m_pResourceProvider->CreateOrUpdateBuffer(idxBufferParams, pViewData->activeIndexBuffer);*/

	/*Renderer::Buffers::SBufferParams vtxBufferParams(DYNAMIC_VB_SIZE, 1, Renderer::Buffers::EBufferBindType::Vertex);
	pViewData->activeVertexBuffer = m_pResourceProvider->CreateOrUpdateBuffer(vtxBufferParams, pViewData->activeVertexBuffer);*/

	pViewData->stage = pStage;

	m_pCurrentView = &*pViewData;

	pViewData->view->GetRenderer()->Init(this);
	m_perViewRenderData.push_back(std::move(pViewData));
	
	
}

void Cry::Ns::CRenderDevice::RT_RenderView(SPerViewRenderData* pViewData, Renderer::Pipeline::StageRenderArguments& args)
{
	m_pCurrentView = pViewData;

	auto pRenderer = pViewData->view->GetRenderer();

	//Fix this to actual draw the last frame without clearing it when no data is available
	pRenderer->UpdateRenderTree();
	//if ()
	{
		pRenderer->RenderOffscreen();
		pRenderer->Render();
	}
	//TODO: Replace with proper api
	m_pPipeline->RT_StretchToColorTarget(m_pCurrentView->viewColorTarget, gBlendStateMask[1] | gDepthStateMasks[0]);


	m_pCurrentView = nullptr;
}

void Cry::Ns::CRenderDevice::RT_DestroyView(SPerViewRenderData* pViewData, Renderer::Pipeline::StageDestructionsArguments& args)
{
	for (auto iter = m_perViewRenderData.begin(); iter != m_perViewRenderData.end(); ++iter)
	{
		if (iter->get() != pViewData)
			continue;

		m_pResourceProvider->FreeBuffer(pViewData->activeIndexBuffer);
		m_pResourceProvider->FreeBuffer(pViewData->activeVertexBuffer);

		pViewData->view->GetRenderer()->Shutdown();

		m_perViewRenderData.erase(iter);	


	}
}