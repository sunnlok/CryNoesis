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
			virtual const ::Noesis::DeviceCaps& GetCaps() const override;
			virtual ::Noesis::Ptr<::Noesis::RenderTarget> CreateRenderTarget(const char* label, uint32_t width, uint32_t height, uint32_t sampleCount, bool needsStencil) override;
			virtual ::Noesis::Ptr<::Noesis::RenderTarget> CloneRenderTarget(const char* label, ::Noesis::RenderTarget* surface) override;
			virtual ::Noesis::Ptr<::Noesis::Texture> CreateTexture(const char* label, uint32_t width, uint32_t height, uint32_t numLevels, ::Noesis::TextureFormat::Enum format, const void** data) override;
			virtual void UpdateTexture(::Noesis::Texture* texture, uint32_t level, uint32_t x, uint32_t y, uint32_t width, uint32_t height, const void* data) override;

			virtual void BeginOffscreenRender() override;
			virtual void SetRenderTarget(::Noesis::RenderTarget* surface) override;

			virtual void ResolveRenderTarget(::Noesis::RenderTarget* surface, const ::Noesis::Tile* tiles, uint32_t numTiles) override;
			virtual void EndOffscreenRender() override;

			/// Begins rendering onscreen commands
			virtual void BeginOnscreenRender() override;

			/// Ends rendering onscreen commands
			virtual void EndOnscreenRender() override;

			virtual void* MapVertices(uint32_t bytes) override;
			virtual void UnmapVertices() override;

			virtual void* MapIndices(uint32_t bytes) override;
			virtual void UnmapIndices() override;


			virtual void DrawBatch(const ::Noesis::Batch& batch) override;
			//~RenderDevice		


			void UpdateViewSize(ViewRenderData* perViewData, uint32 width, uint32 height);


			std::unique_ptr<ViewRenderData> InitializeRenderViewData(ViewData &viewData);
			void DestroyView(TRenderViewDataPtr pRenderData, ::Noesis::Ptr<::Noesis::IView> pView);



		protected:


			void UpdateViewRenderTargets(ViewRenderData* perViewData, int newWidth, int newHeight);



			void BeginActualRender();
			void EndActualRender();

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