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
		struct ViewData;
		struct ViewRenderData;

		class CRenderTarget;

		struct SShaderInfo
		{
			::Noesis::Shader	effectID;
			uint64	mask;
			int		layout;
			_smart_ptr<IShader> pShader;	

			bool operator ==(const SShaderInfo& info) { return info.effectID.v == effectID.v; }
			bool operator ==(::Noesis::Shader id) { return id.v == effectID.v; }
		};
		using TShaderInfoList = std::vector<SShaderInfo>;

		using TRenderViewDataPtr = std::unique_ptr<ViewRenderData>;

		class CRenderDevice final : public ::Noesis::RenderDevice
		{
		public:
			static CRenderDevice* Get();


			CRenderDevice();
			virtual ~CRenderDevice();
	
			//RenderDevice
			const ::Noesis::DeviceCaps& GetCaps() const override;
			::Noesis::Ptr<::Noesis::RenderTarget> CreateRenderTarget(const char* label, uint32_t width, uint32_t height, uint32_t sampleCount) override;
			::Noesis::Ptr<::Noesis::RenderTarget> CloneRenderTarget(const char* label, ::Noesis::RenderTarget* surface) override;
			::Noesis::Ptr<::Noesis::Texture> CreateTexture(const char* label, uint32_t width, uint32_t height, uint32_t numLevels, ::Noesis::TextureFormat::Enum format, const void** data) override;
			void UpdateTexture(::Noesis::Texture* texture, uint32_t level, uint32_t x, uint32_t y, uint32_t width, uint32_t height, const void* data) override;

			void BeginRender(bool offscreen) override;
			void SetRenderTarget(::Noesis::RenderTarget* surface) override;

			void BeginTile(const ::Noesis::Tile& tile, uint32_t surfaceWidth, uint32_t surfaceHeight) override;
			void EndTile() override;

			void ResolveRenderTarget(::Noesis::RenderTarget* surface, const ::Noesis::Tile* tiles, uint32_t numTiles) override;
			void EndRender() override;

			void* MapVertices(uint32_t bytes) override;
			void UnmapVertices() override;

			void* MapIndices(uint32_t bytes) override;
			void UnmapIndices() override;


			virtual void DrawBatch(const ::Noesis::Batch& batch) override;
			//~RenderDevice		


			void UpdateViewSize(ViewRenderData* perViewData, uint32 width, uint32 height);


			std::unique_ptr<ViewRenderData> InitializeRenderViewData(ViewData &viewData);
			void DestroyView(TRenderViewDataPtr pRenderData, ::Noesis::Ptr<::Noesis::IView> pView);
		protected:


			void UpdateViewRenderTargets(ViewRenderData* perViewData, int newWidth, int newHeight);



			void BeginActualRender();
			void EndActualRender();
			void BeginOffscreenRender();
			void EndOffscreenRender();

			void RT_CheckAndUpdateViewTarget(Cry::Ns::ViewRenderData &ViewData);

			void RT_InitializeViewRenderer(ViewRenderData& viewRenderData, ViewData& viewData);

			void RT_DestroyView(ViewRenderData* pRenderViewData);
			void RT_RenderView(ViewRenderData* pViewData, Cry::Renderer::Pipeline::StageRenderArguments& args);
			void RT_DestroyView(ViewRenderData* pViewData, Cry::Renderer::Pipeline::StageDestructionsArguments& args);

			Cry::Renderer::IStageResourceProvider* m_pResourceProvider;


			Cry::Renderer::Pipeline::ICustomPipelinePtr m_pPipeline;


			ViewRenderData* m_pCurrentView = nullptr;

			bool m_bRenderingOffscreen = false;

			IShader* m_pNoesisShader;

			CRenderTarget* m_pCurrentRenderTarget;			

			uint32 m_viewID = 0;
		};
	}
}