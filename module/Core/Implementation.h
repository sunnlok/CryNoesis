#pragma once
#include "Renderer/Renderer.h"
#include "INoesis.h"
#include "ViewManager.h"


namespace Cry
{
namespace Ns
{
	using TViewList = std::vector<Noesis::Ptr<Noesis::IView>>;

	class CInputHandler;

	class CImplementation : public Cry::INoesis
	{
	public:
		static CImplementation* Instantiate();
		static void Destroy();
		static CImplementation* Get();
	public:
		CImplementation();
		~CImplementation();

		void Init();

		void Update(float delta);
		void UpdateBeforeRender();
		void OnScreenSizeChanged();

		void RegisterVariables();
		void LoadResources();

		bool CreateView(const char* xamlPath, Vec2i dimensions);

		virtual ViewManager* GetViewManager() const final;
	private:
		ICVar* m_pResourceDictVar;


		float m_lastFrameDelta = 0;


		Noesis::Ptr<CRenderDevice> m_pRenderDevice;
		CTimeValue m_startTime;

		std::unique_ptr<CInputHandler> m_pInputHandler;
		std::unique_ptr<ViewManager> m_pViewManager;
	};
}
}