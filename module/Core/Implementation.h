#pragma once
#include "Renderer/Renderer.h"


namespace Cry
{
namespace Ns
{
	using TViewList = std::vector<Noesis::Ptr<Noesis::IView>>;

	class CInputHandler;

	class CImplementation
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

		bool CreateView(const char* xamlPath, Vec2i dimensions);

		TViewList& GetViewList() { return m_views; }
	private:
		float m_lastFrameDelta = 0;

		TViewList m_views;

		Noesis::Ptr<CRenderDevice> m_pRenderDevice;
		CTimeValue m_startTime;

		std::unique_ptr<CInputHandler> m_pInputHandler;
	};
}
}