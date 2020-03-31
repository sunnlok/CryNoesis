#pragma once
#include "CryRenderer/IRenderer.h"
#include "NsRender/RenderDevice.h"
#include "CryThreading/CryThread.h"
#include <CryRenderer/Pipeline/IPipeline.h>
#include <atomic>
#include "CryRenderer/Pipeline/IStageResources.h"

namespace Noesis
{
	struct IView;
}

namespace Cry
{
	namespace Ns
	{

		struct SViewInitParams
		{
			Noesis::Ptr<Noesis::IView> view;

			int width;
			int height;
		};

		struct SShaderInfo
		{
			Noesis::Shader	effectID;
			uint64	mask;
			int		layout;
			_smart_ptr<IShader> pShader;	

			bool operator ==(const SShaderInfo& info) { return info.effectID.v == effectID.v; }
			bool operator ==(Noesis::Shader id) { return id.v == effectID.v; }
		};
		using TShaderInfoList = std::vector<SShaderInfo>;


		struct SPerViewRenderData
		{
			Noesis::Ptr<Noesis::IView>			view;
			Renderer::Pipeline::TStageBasePtr	stage;

			uint32								viewPass;

			_smart_ptr<ITexture>				viewColorTarget;
			_smart_ptr<ITexture>				viewDepthTarget;

			uintptr_t activeIndexBuffer		=	Renderer::Buffers::CINVALID_BUFFER;
			uint32 idxBufferOffset = 0;
			uint32 idxBufferDrawPos = 0;

			uintptr_t activeVertexBuffer	=	Renderer::Buffers::CINVALID_BUFFER;
			uint32 vtxBufferOffset = 0;
			uint32 vtxBufferDrawPos = 0;

			//This is not ideal. Sadly there is no way to track constant buffers outside the renderer via smart pointer
			uintptr_t vertexCB = Renderer::Buffers::CINVALID_BUFFER;
			uintptr_t effectCB = Renderer::Buffers::CINVALID_BUFFER;
			uintptr_t pixelCB = Renderer::Buffers::CINVALID_BUFFER;
			uintptr_t textDimCB = Renderer::Buffers::CINVALID_BUFFER;

			uint32_t vertexCBHash;
			uint32_t pixelCBHash;
			uint32_t effectCBHash;
			uint32_t texDimensionsCBHash;
		};
		using TViewDataPtr = std::unique_ptr<SPerViewRenderData>;
		using TViewDataList = std::vector<TViewDataPtr>;

		class CRenderDevice final : public Noesis::RenderDevice
		{
		public:
			CRenderDevice();
			virtual ~CRenderDevice();
	
			//RenderDevice
			const Noesis::DeviceCaps& GetCaps() const override;
			Noesis::Ptr<Noesis::RenderTarget> CreateRenderTarget(const char* label, uint32_t width, uint32_t height, uint32_t sampleCount) override;
			Noesis::Ptr<Noesis::RenderTarget> CloneRenderTarget(const char* label, Noesis::RenderTarget* surface) override;
			Noesis::Ptr<Noesis::Texture> CreateTexture(const char* label, uint32_t width, uint32_t height, uint32_t numLevels, Noesis::TextureFormat::Enum format, const void** data) override;
			void UpdateTexture(Noesis::Texture* texture, uint32_t level, uint32_t x, uint32_t y, uint32_t width, uint32_t height, const void* data) override;

			void BeginRender(bool offscreen) override;
			void SetRenderTarget(Noesis::RenderTarget* surface) override;

			void BeginTile(const Noesis::Tile& tile, uint32_t surfaceWidth, uint32_t surfaceHeight) override;
			void EndTile() override;

			void ResolveRenderTarget(Noesis::RenderTarget* surface, const Noesis::Tile* tiles, uint32_t numTiles) override;
			void EndRender() override;

			void* MapVertices(uint32_t bytes) override;
			void UnmapVertices() override;

			void* MapIndices(uint32_t bytes) override;
			void UnmapIndices() override;


			virtual void DrawBatch(const Noesis::Batch& batch) override;
			//~RenderDevice		

			void AddView(SViewInitParams viewParams);

		protected:
			Renderer::Pipeline::Pass::SInlineConstantParams CreateConstantParameters(const Noesis::Batch& batch);


			void BeginActualRender();
			void EndActualRender();
			void BeginOffscreenRender();
			void EndOffscreenRender();

			void RT_AddView(SViewInitParams viewParams);

			void RT_RenderView(SPerViewRenderData* pViewData, Renderer::Pipeline::StageRenderArguments& args);
			void RT_DestroyView(SPerViewRenderData* pViewData, Renderer::Pipeline::StageDestructionsArguments& args);


			Renderer::Pipeline::TStageBasePtr m_offscreenStage;
			Renderer::Pipeline::TStageBasePtr m_mainStage;

			std::array<uintptr_t, 2> m_currentBuffer = { Renderer::Buffers::CINVALID_BUFFER, Renderer::Buffers::CINVALID_BUFFER};

			std::array<std::vector<uintptr_t>, 2> m_usedBuffers;
			Renderer::IStageResourceProvider* m_pResourceProvider;


			Renderer::Pipeline::ICustomPipelinePtr m_pPipeline;

			TViewDataList m_perViewRenderData;

			SPerViewRenderData* m_pCurrentView = nullptr;

			bool m_bRenderingOffscreen = false;

			uint32 m_viewID = 0;

			IShader* m_pNoesisShader;

			Noesis::RenderTarget* m_pCurrentRenderTarget;
		};
	}
}